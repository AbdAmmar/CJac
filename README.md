# Poisson's Equation Solver

## Overview

The code solves the 2D Poisson's equation of the form:

$$
\nabla^2 u = f
$$

where $u$ is the potential function to be solved for, and $f$ is a known function representing the source term. 
The problem is solved using a finite difference method, and the parallelism is achieved through a combination of 
MPI and OpenMP to handle large-scale computations efficiently.

## Dependencies

- **MPI**: Message Passing Interface library (e.g., MPICH, OpenMPI)
- **OpenMP**: Open Multi-Processing for parallel computing on shared-memory systems
- **C Compiler**: A C compiler that supports MPI and OpenMP (e.g., gcc)

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
