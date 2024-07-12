#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "init_kernel.h"
#include "comp_kernel.h"
#include "comm_kernel.h"
#include "solution_kernel.h"

int main() {

    int n;
    int ntx, nty;
    int n_Workers, nty_local;

    size_t size_u; 

    int i, j, ii, jj, jj0, jj1, l;
    int it, it_max, it_print;

    double L, h;

    double* u;

    double* err_i;
    double err;

    FILE *fptr;
    char readString[100];

    fptr = fopen("param.txt", "r");
    if(fptr != NULL) {
        // read n
        fgets(readString, 100, fptr);
        n = atoi(readString);

        // read n_Workers
        fgets(readString, 100, fptr);
        n_Workers = atoi(readString);

        // read it_max
        fgets(readString, 100, fptr);
        it_max = atoi(readString);

        // read it_print
        fgets(readString, 100, fptr);
        it_print = atoi(readString);

        //while(fgets(readString, 100, fptr)) {
        //    printf("%s", readString);
        //}
    } else {
        printf("Not able to open the file param.txt\n");
    }
    fclose(fptr);


    //n = 128;
    L = 1.0;
    h = L / (double) (n-1);

    printf("nb on grid points = %d x %d\n", n, n);
    printf("dim of grid = %.1f x %.1f\n", L, L);
    printf("step = %f\n\n", h);


    // MPI CPUs
    //n_Workers = 4;
    printf("nb on workers = %d\n", n_Workers);


    ntx = n;
    nty = n + 2*n_Workers;
    nty_local = n / n_Workers + 2;
    if((nty_local*n_Workers - nty) != 0) {
        printf("Unconsistent dimensions\n");
        exit(0);
    }

    printf("ntx = %d\n", ntx);
    printf("nty = %d\n", nty);
    printf("nty_local = %d\n\n", nty_local);


    size_u = ntx * nty * sizeof(double);

    u = (double*) malloc(size_u);
    if(u == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }
    printf("Size of u = %zu Bytes \n\n", size_u);


    err_i = (double*) malloc(n_Workers * sizeof(double));


    it = 0;
    //it_max = 100;
    //it_print = it_max/10;
    while(it < it_max) {
        it++;
        for(i = 0; i < n_Workers; i++) {
            compute(i, ntx, nty, nty_local, h, u);
            err_i[i] = max_error(i, ntx, nty, nty_local, h, u); 
            communication(i, ntx, nty, nty_local, u);
        }
        err = err_i[0];
        for(i = 1; i < n_Workers; i++) {
            if(err < err_i[i]) {
                err = err_i[i];
            }
        }
        if(it%it_print == 0)
            printf("it = %d/%d, error = %f\n", it, it_max, err);

        //for(l = 0; l < n_Workers; l++){
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


    free(u);

    return 0;
}

