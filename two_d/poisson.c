#include <stdlib.h>
#include <stdio.h>

#include "init_kernel.h"
#include "comp_kernel.h"

int main() {

    int n;
    int ntx, nty;
    int n_Workers, nty_local;

    size_t size_u; 

    int i, j, ii, jj, jj0, jj1, l;
    int it, it_max;

    double L, h;

    double* u;

    n = 8;
    L = 1.0;
    h = L / (double) (n-1);

    printf("nb on grid points = %d x %d\n", n, n);
    printf("dim of grid = %.1f x %.1f\n", L, L);
    printf("step = %f\n\n", h);


    // MPI CPUs
    n_Workers = 4;
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


    //for(i = 0; i < n_Workers; i++){
    //    initalize(i, ntx, nty_local, n_Workers, h, u);
    //}
    //for (j = 0; j < nty; j++) {
    //    jj = j * ntx;
    //    for (i = 0; i < ntx; i++) {
    //        ii = jj + i;
    //        u[ii] = 0.0;
    //        printf("u[%d] = %f  ", ii, u[ii]);
    //    }
    //    printf("\n");
    //}

    it = 0;
    it_max = 1;
    while(it < it_max) {
        it++;
        printf("it = %d/%d\n", it, it_max);
        for(i = 0; i < n_Workers; i++){
            compute(i, ntx, nty, nty_local, h, u);
            // communication(ntx, nty_local, n_Workers, u);
        }

        //for(i = 0; i < n_Workers; i++){
        //    jj0 = nty_local * i;
        //    for (j = 1; j < nty_local-1; j++) {
        //        jj1 = jj0 + j * ntx;
        //        for (i = 0; i < ntx; i++) {
        //            ii = jj1 + i;
        //            printf("u[%d] = %f  ", ii, u[ii]);
        //        }
        //        printf("\n");
        //    }
        //}

        for(l = 0; l < n_Workers; l++){
            jj0 = l * nty_local;
            for (j = 1; j < nty_local-1; j++) {
                jj1 = (jj0 + j) * ntx;
                //printf("%d %d %d\n", l, jj0, jj1);
                for (i = 0; i < ntx; i++) {
                    ii = jj1 + i;
                    //printf("u[%d] = %f  ", ii, u[ii]);
                    printf("%f  ", u[ii]);
                }
                printf("\n");
            }
            printf("\n");
        }

        printf("\n");
    }


    free(u);

    return 0;
}

