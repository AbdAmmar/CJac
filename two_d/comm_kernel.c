
void communication(int tid, int ntx, int nty, int nty_local, double *u) {

    int l;
    int jj0, jj1, jj2;
    double tmp;

    jj0 = nty_local * tid;

    if(jj0 != 0) {
        jj1 = (jj0 - 1) * ntx;
        jj2 = jj1 + ntx;
        //printf("TOP communication on %d %d\n", jj1, jj2);
        for(l = 0; l < ntx; l++) {
            //printf("%d <-> %d  ", jj1+l, jj2+l);
            tmp = u[jj1 + l];
            u[jj1 + l] = u[jj2 + l];
            u[jj2 + l] = tmp;
        }
        //printf("\n");
    }

    //if((jj0 + nty_local != nty) && (jj0 > 0)) {
    //    jj1 = (jj0 + nty_local) * ntx;
    //    jj2 = jj1 + ntx;
    //    printf("BOTTOM communication on %d %d\n", jj1, jj2);
    //    for(l = 0; l < ntx; l++) {
    //        printf("%d <-> %d  ", jj1-l-1, jj2-l-1);
    //        tmp = u[jj1 - l - 1];
    //        u[jj1 - l - 1] = u[jj2 - l - 1];
    //        u[jj2 - l - 1] = tmp;
    //    }
    //    printf("\n");
    //}

}

