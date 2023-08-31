#!/bin/bash -l
#SBATCH --time=0:01:00           # ask for 1 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH --ntasks-per-node=4      # only use four cores per node
#SBATCH --nodes=1                # only use one node

module load foss

mpirun /homes/ronnyim12345/3way-mpi/build/hw4_mpi


