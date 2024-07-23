
#include <math.h>


double solution(double x, double y) {

    return x * y * (x - 1.0) * (y - 1.0);

}


double max_error(int tid, int ntx, int nty, int nty_local, double h, double *u) {

    int l;
    int j, jj0, jj1, jj2, jy;

    double x, y;
    double tmp, err;

    jj0 = nty_local * tid;
    jy = jj0 - 2 * tid - 1;

    err = 0.0;
    for(j = 1; j < nty_local-1; j++) {

        y = (double) (jy + j) * h;

        jj1 = j * ntx;
        jj2 = (jj0 + j) * ntx;

        for(l = 1; l < ntx-1; l++) {

            x = (double) l * h;

            tmp = fabs(u[jj2 + l] - solution(x, y));
            if(tmp > err) {
                err = tmp;
            }
        }
    }

    return err;

}

