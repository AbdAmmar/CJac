#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "init_kernel.h"
#include "comp_kernel.h"
#include "solution_kernel.h"

int main() {

    int n;
    int ntx, nty;
    int nWorkers, nty_local;

    size_t size_u; 

    int i, j, ii, jj, jj0, jj1, l;
    int it, it_max, it_print;

    double L, h;

    double* u;
    double* u_new;

    double err, err_local;

    FILE *fptr;
    char readString[100];

    int rang;

    MPI_Comm comm2d;
    int dims[2], periods[2], neighbor[2];


    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &nWorkers);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    //printf("Je suis le processus %d parmi %d\n", rang, nWorkers);

    if(rang == 0) {

        fptr = fopen("param.txt", "r");
        if(fptr != NULL) {

            if(fgets(readString, 100, fptr) != NULL) {
                n = atoi(readString);
            } else {
                printf("Not able to read n\n");
            }

            if(fgets(readString, 100, fptr) != NULL) {
                it_max = atoi(readString);
            } else {
                printf("Not able to read it_max\n");
            }

            if(fgets(readString, 100, fptr) != NULL) {
                it_print = atoi(readString);
            } else {
                printf("Not able to read it_print\n");
            }

        } else {
            printf("Not able to open the file param.txt\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
        fclose(fptr);


        L = 1.0;
        h = L / (double) (n-1);

        printf("nb on grid points = %d x %d\n", n, n);
        printf("dim of grid = %.1f x %.1f\n", L, L);
        printf("step = %f\n\n", h);


        printf("nb on workers = %d\n", nWorkers);


        ntx = n;
        nty = n + 2 * nWorkers;
        nty_local = n / nWorkers + 2;
        if(nty != nty_local*nWorkers) {
            printf("Unconsistent dimensions\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        printf("ntx = %d\n", ntx);
        printf("nty = %d\n", nty);
        printf("nty_local = %d\n\n", nty_local);


        size_u = ntx * nty * sizeof(double);
        printf("Size of u = %zu Bytes \n\n", size_u);

    } // end if(rang == 0)


    MPI_Bcast(&ntx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nty, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nty_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nWorkers, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&it_max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&it_print, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    u = (double*) malloc(ntx * nty_local * sizeof(double));
    if(u == NULL) {
        fprintf(stderr, "Memory allocation failed for u\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    u_new = (double*) malloc(ntx * nty_local * sizeof(double));
    if(u_new == NULL) {
        fprintf(stderr, "Memory allocation failed for u_new\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }


    init(ntx, nty_local, u);

    dims[0] = 1;
    dims[1] = nWorkers;
    periods[0] = 0;
    periods[1] = 0;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm2d);
    MPI_Cart_shift(comm2d, 1, 1, &neighbor[0], &neighbor[1]);
    //printf("I process %d, my neighbors are : %d-%d\n", rang, neighbor[0], neighbor[1]);


    it = 1;
    while(it <= it_max) {

        if(it%2 != 0) {
            compute(rang, ntx, nty, nty_local, h, u, u_new);

            MPI_Sendrecv(&u_new[0],                     ntx, MPI_DOUBLE, neighbor[0], 0, 
                         &u_new[(nty_local - 2) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         comm2d, MPI_STATUS_IGNORE);

            MPI_Sendrecv(&u_new[(nty_local - 1) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         &u_new[ntx],                   ntx, MPI_DOUBLE, neighbor[0], 0, 
                         comm2d, MPI_STATUS_IGNORE);

        } else {

            compute(rang, ntx, nty, nty_local, h, u_new, u);

            MPI_Sendrecv(&u[0],                     ntx, MPI_DOUBLE, neighbor[0], 0, 
                         &u[(nty_local - 2) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         comm2d, MPI_STATUS_IGNORE);

            MPI_Sendrecv(&u[(nty_local - 1) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         &u[ntx],                   ntx, MPI_DOUBLE, neighbor[0], 0, 
                         comm2d, MPI_STATUS_IGNORE);

        }

        if(it%it_print == 0) {
            err_local = max_error(rang, ntx, nty_local, h, u); 
            MPI_Allreduce(&err_local, &err, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD); 
            if(rang == 0) {
                printf("it = %d/%d, error = %f\n", it, it_max, err);
            }
        }

        it++;

        //for(l = 0; l < nWorkers; l++){
        //    jj0 = l * nty_local;
        //    for (j = 1; j < nty_local-1; j++) {
        //        jj1 = (jj0 + j) * ntx;
        //        for (i = 0; i < ntx; i++) {
        //            ii = jj1 + i;
        //            printf("%f  ", u[ii]);
        //        }
        //        printf("\n");
        //    }
        //    printf("\n");
        //}
        //printf("\n");
    }

    //for(l = 0; l < nWorkers; l++){
    //    jj0 = l * nty_local;
    //    for (j = 1; j < nty_local-1; j++) {
    //        jj1 = (jj0 + j) * ntx;
    //        for (i = 0; i < ntx; i++) {
    //            ii = jj1 + i;
    //            printf("%f  ", u[ii]);
    //        }
    //        printf("\n");
    //    }
    //    printf("\n");
    //}
    //printf("\n");


    free(u);
    free(u_new);

    MPI_Finalize();

    return 0;
}

