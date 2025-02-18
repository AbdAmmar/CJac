#ifndef COMP_KERNEL
#define COMP_KERNEL

void compute(int tid, int ntx, int nty, int nty_local, double h, double* u_old, double *u_new);

void compute_row(int j, int tid, int ntx, int nty, int nty_local, double h, double* u_old, double *u_new);

void compute_row_bc(int tid, int ntx, int nty, int nty_local, double h, double *u_new);

#endif
