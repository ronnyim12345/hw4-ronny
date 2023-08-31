#!/bin/bash -l
#SBATCH --time=0:030:00           # ask for 30 minute runtime
#SBATCH --constraint=moles        # only run on moles
#SBATCH --ntasks-per-node=20      # only use 20 cores per node
#SBATCH --nodes=4                 # only use four node

module load foss

mpirun /homes/ronnyim12345/3way-mpi/build/hw4_mpi


