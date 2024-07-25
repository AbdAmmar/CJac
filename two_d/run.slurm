#!/bin/bash
#SBATCH -J jacobi
#SBATCH --nodes=15
#SBATCH --ntasks=1200
###SBATCH --cpus-per-task=1
#SBATCH --time=1:00:00
###SBATCH -p small 
###SBATCH -p big
#SBATCH -p full
#SBATCH --exclusive 


module purge
module load openmpi/arm/4.1.4-cpu 


mpirun -np $SLURM_NTASKS ./poisson

