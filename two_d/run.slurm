#!/bin/bash
#SBATCH -J jacobi
#SBATCH --nodes=1
#SBATCH --ntasks=40
#SBATCH --cpus-per-task=1
#SBATCH --time=1:00:00
#SBATCH -p small 
#SBATCH --exclusive 


module purge
module load openmpi


mpirun -np $SLURM_NTASKS ./poisson

