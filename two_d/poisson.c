#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


int main() {

    int nx, ny;
    int ntx, nty;
    int n_Workers, nty_local;
    int n_MPI;

    size_t size_u; 

    int it, it_max;

    double Lx, h;

    double* u;
    double* f;


    nx = 1024;
    ny = 1024;

    Lx = 1.0;

    h = Lx / double(nx);

    n_Workers = n_MPI;

    nty_local = ny / n_Workers;

    if((nty_local*n_Workers - ny) != 0)
        break;

    ntx = nx + 2;
    nty = ny + 2*n_Workers;

    it_max = 10;

    size_u = ntx * nty * sizeof(double);

    u = (double*) malloc(size_u);
    if(u == NULL) {
        printf("Memory not allocated for u.\n");
    } 
    f = (double*) malloc(size_u);

    call initalize_kernel(ntx, nty_local, n_Workers, h, f, u);

    it = 0;
    while(it < it_max) {
        it++;
        call communication_kernel(ntx, nty_local, n_Workers, u);
        call compute_kernel(ntx, nty_local, n_Workers, h, f, u);
    }



    return 0;
}

