#!/bin/bash -l
#SBATCH --time=0:05:00           # ask for 5 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH	--cpus-per-task=10

module load foss

export OMP_NUM_THREADS=10
srun /homes/ronnyim12345/3way-openmp/build/hw4_openmp


