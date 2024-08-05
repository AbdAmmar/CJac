#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <omp.h>

#include "utils.h"
#include "init_kernel.h"
#include "comp_kernel.h"
#include "solution_kernel.h"

int main() {

    int n;
    int ntx, nty;
    int nWorkers, nty_local;

    int j;

    size_t size_u; 

    int it, it_max, it_print;

    double L, h;

    double* u;
    double* u_new;

    double err, err_local;

    FILE *fptr;
    char readString[100];

    int rank;
    double start_time, end_time, total_time;
    double cpu_time, cpu_total_time;
    clock_t cpu_start_time, cpu_end_time;


    MPI_Comm comm2d;
    int dims[2], periods[2], neighbor[2];



    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &nWorkers);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    cpu_start_time = clock();


    //printf("Je suis le processus %d parmi %d\n", rank, nWorkers);

    if(rank == 0) {

        start_time = MPI_Wtime();

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

    } // end if(rank == 0)


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
    //printf("I process %d, my neighbors are : %d-%d\n", rank, neighbor[0], neighbor[1]);


    it = 1;
    while(it <= it_max) {

        if(it%2 != 0) {

            #pragma omp parallel \
                default(none)    \
                private(j)       \
                shared(rank, ntx, nty, nty_local, h, u, u_new)
            for(j = 1; j < nty_local-1; j++) {
                compute_row(j, rank, ntx, nty, nty_local, h, u, u_new);
            }

            compute_row_bc(rank, ntx, nty, nty_local, h, u_new);

            MPI_Sendrecv(&u_new[0],                     ntx, MPI_DOUBLE, neighbor[0], 0, 
                         &u_new[(nty_local - 2) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         comm2d, MPI_STATUS_IGNORE);

            MPI_Sendrecv(&u_new[(nty_local - 1) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         &u_new[ntx],                   ntx, MPI_DOUBLE, neighbor[0], 0, 
                         comm2d, MPI_STATUS_IGNORE);

        } else {

            #pragma omp parallel \
                default(none)    \
                private(j)       \
                shared(rank, ntx, nty, nty_local, h, u, u_new)
            for(j = 1; j < nty_local-1; j++) {
                compute_row(j, rank, ntx, nty, nty_local, h, u_new, u);
            }

            compute_row_bc(rank, ntx, nty, nty_local, h, u);

            MPI_Sendrecv(&u[0],                     ntx, MPI_DOUBLE, neighbor[0], 0, 
                         &u[(nty_local - 2) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         comm2d, MPI_STATUS_IGNORE);

            MPI_Sendrecv(&u[(nty_local - 1) * ntx], ntx, MPI_DOUBLE, neighbor[1], 0, 
                         &u[ntx],                   ntx, MPI_DOUBLE, neighbor[0], 0, 
                         comm2d, MPI_STATUS_IGNORE);

        }

        if(it%it_print == 0) {
            err_local = max_error(rank, ntx, nty_local, h, u); 
            MPI_Allreduce(&err_local, &err, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD); 
            if(rank == 0) {
                printf("it = %d/%d, error = %f\n", it, it_max, err);
            }
        }

        it++;

    }

    //print_mat(ntx, nty_local, nWorkers, u);

    free(u);
    free(u_new);

    MPI_Barrier(MPI_COMM_WORLD);

    cpu_end_time = clock();
    cpu_time = ((double) (cpu_end_time - cpu_start_time)) / CLOCKS_PER_SEC;
    MPI_Reduce(&cpu_time, &cpu_total_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        end_time = MPI_Wtime();
        total_time = end_time - start_time;

        printf("\nTotal wall-clock computation time: %f seconds\n", total_time);
        printf("Total CPU computation time: %f seconds\n", cpu_total_time);
    }

    MPI_Finalize();

    return 0;
}


