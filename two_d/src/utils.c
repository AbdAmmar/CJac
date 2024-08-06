
#include <stdio.h>

void print_mat(int ntx, int nty_local, int nWorkers, double *u) {

    int l;
    int j, jj0, jj1;
    int i;

    for(l = 0; l < nWorkers; l++){

        jj0 = l * nty_local;

        for (j = 1; j < nty_local-1; j++) {

            jj1 = (jj0 + j) * ntx;

            for (i = 0; i < ntx; i++) {

                printf("%f  ", u[jj1 + i]);

            }

            printf("\n");

        }

        printf("\n");

    }

    printf("\n");

}
