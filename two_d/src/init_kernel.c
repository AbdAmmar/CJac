
void init(int ntx, int nty_local, double *u) {

    int l;
    int j, jj1;

    for(j = 0; j < nty_local; j++) {

        jj1 = j * ntx;

        for(l = 0; l < ntx; l++) {

            u[jj1 + l] = 0.0;

        }
    }

}


