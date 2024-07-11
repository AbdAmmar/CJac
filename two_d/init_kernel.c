
void initalize(int tid, int ntx, int nty_local, int n_Workers, double h, double *u) {


    int j, k, l;
    int ii0, ii1, ii2;
    int ll0, ll1, ll2, ll3;

    ii0 = nty_local * tid;
    ii1 = ii0 + nty_local + 1;
    ii2 = 2 * tid;

    for(j = 1; j < nty_local; j++) {

        k = ii0 + j;

        ll0 = (k + ii2) * ntx;
        for(l = 0; l < ntx; l++) {

            ll1 = l + ll0;

            u[ll1] = 0.0;
        }

        ll0 = ii0 + ii2;
        ll1 = ii1 + ii2;
        for(l = 0; l < ntx; l++) {

            ll2 = l + ll0;
            ll3 = l + ll1;

            u[ll2] = u[ll2+1];
            u[ll3] = u[ll3-1];
        }
    }

}
