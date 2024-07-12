#ifndef SOLUTION_KERNEL
#define SOLUTION_KERNEL

double solution(double x, double y);

double max_error(int tid, int ntx, int nty, int nty_local, double h, double *u);

#endif
