
void communication(int tid, int ntx, int nty_local, double *u) {

    int l;
    int jj0, jj1, jj2;
    double tmp;

    jj0 = nty_local * tid;

    if(jj0 != 0) {
        jj1 = (jj0 - 1) * ntx;
        jj2 = jj1 + ntx;
        for(l = 0; l < ntx; l++) {
            tmp = u[jj1 + l];
            u[jj1 + l] = u[jj2 + l];
            u[jj2 + l] = tmp;
        }
    }

}

