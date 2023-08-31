#!/bin/bash -l
#SBATCH --time=0:05:00           # ask for 5 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH --ntasks-per-node=20      # only use 10 cores per node
#SBATCH --nodes=2                # only use one node

module load foss

mpirun /homes/ronnyim12345/3way-mpi/build/hw4_mpi


