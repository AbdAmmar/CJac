#!/bin/bash
#SBATCH -J jacobi

#SBATCH --nodes=1 --exclusive 

#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1

#SBATCH --cpus-per-task=80

#SBATCH --time=1:00:00
#SBATCH -p small
###SBATCH -p big
###SBATCH -p full


module purge
module load openmpi/arm/4.1.4-cpu 

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


echo "nb of nodes = $SLURM_NNODES"
echo "nb of tasks per node = $SLURM_NTASKS_PER_NODE"
echo "nb of tasks = $SLURM_NTASKS"
echo "nb of threads per task = $OMP_NUM_THREADS"

mpirun --bind-to none -np $SLURM_NTASKS ./poisson
#./poisson

