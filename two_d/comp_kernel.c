#include <stdio.h>
#include <stdlib.h>

#include "source_kernel.h"

void compute(int tid, int ntx, int nty, int nty_local, double h, double* u_old, double *u_new) {

    int l, ll;
    int j, jj0, jj1, jj2, jy;

    double x, y, h2;

    jj0 = nty_local * tid;

    jy = jj0 - 2 * tid - 1;
    h2 = h * h;

    for(j = 1; j < nty_local-1; j++) {

        y = (double) (jy + j) * h;

        jj1 = j * ntx;

        for(l = 1; l < ntx-1; l++) {

            x = (double) l * h;

            ll = jj1 + l;

            u_new[ll] = 0.25 * (u_old[ll - 1] + u_old[ll + 1] + u_old[ll - ntx] + u_old[ll + ntx] - h2 * source(x, y));
        }
    }



    if(jj0 != 0) {
        for(l = 0; l < ntx; l++) {
            u_new[l] = u_new[ntx + l];
        }
    } else {
        for(l = ntx; l < 2*ntx; l++) {
            u_new[l] = 0.0;
        }
    }

    if(jj0 + nty_local != nty) {
        jj1 = nty_local * ntx;
        jj2 = jj1 - ntx;
        for(l = 0; l < ntx; l++) {
            u_new[jj1 - l - 1] = u_new[jj2 - l - 1];
        }
    } else {
        jj1 = (nty_local - 1) * ntx;
        for(l = 0; l < ntx; l++) {
            u_new[jj1 - l - 1] = 0.0;
        }
    }

}


