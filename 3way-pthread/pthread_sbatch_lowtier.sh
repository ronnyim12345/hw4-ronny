#!/bin/bash -l
#SBATCH --time=0:030:00           # ask for 30 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH	--cpus-per-task=4

module load foss

export OMP_NUM_THREADS=4
srun /homes/ronnyim12345/3way-openmp/build/hw4_openmp
