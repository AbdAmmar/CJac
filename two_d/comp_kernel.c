#include <stdio.h>
#include <stdlib.h>

#include "source_kernel.h"

void compute(int tid, int ntx, int nty, int nty_local, double h, double *u) {

    int l, ll;
    int j, jj0, jj1, jj2, jy;

    double *u_tmp;
    size_t size_tmp;

    double x, y, h2;

    size_tmp = ntx * nty_local * sizeof(double);

    u_tmp = (double*) malloc(size_tmp);
    if(u_tmp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    jj0 = nty_local * tid;

    for(j = 0; j < nty_local; j++) {

        jj1 = j * ntx;
        jj2 = (jj0 + j) * ntx;

        for(l = 0; l < ntx; l++) {

            u_tmp[jj1 + l] = u[jj2 + l];

        }
    }


    jy = jj0 - 2 * tid - 1;
    h2 = h * h;

    for(j = 1; j < nty_local-1; j++) {

        y = (double) (jy + j) * h;
        //printf("y = %f\n", y);

        jj1 = j * ntx;
        jj2 = (jj0 + j) * ntx;

        for(l = 1; l < ntx-1; l++) {

            x = (double) l * h;

            ll = jj1 + l;

            u[jj2 + l] = 0.25 * (u_tmp[ll - 1] + u_tmp[ll + 1] + u_tmp[ll - ntx] + u_tmp[ll + ntx] - h2 * source(x, y));
        }
    }

    if(jj0 != 0) {
        jj1 = (jj0 + 1) * ntx;
        jj2 = jj1 - ntx;
        for(l = 0; l < ntx; l++) {
            u[jj2 + l] = u[jj1 + l];
        }
    } else {
        for(l = 0; l < ntx; l++) {
            u[ntx + l] = 0.0;
        }
    }

    if(jj0 + nty_local != nty) {
        jj1 = (jj0 + nty_local) * ntx;
        jj2 = jj1 - ntx;
        for(l = 0; l < ntx; l++) {
            u[jj1 - l - 1] = u[jj2 - l - 1];
        }
    } else {
        jj1 = (nty - 1) * ntx;
        for(l = 0; l < ntx; l++) {
            u[jj1 - l - 1] = 0.0;
        }
    }

}


