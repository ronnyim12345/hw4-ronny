#!/bin/bash -l
#SBATCH --time=0:030:00           # ask for 30 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH	--cpus-per-task=10

module load foss

srun /homes/ronnyim12345/3way-pthread/build/hw4_pthread 10

