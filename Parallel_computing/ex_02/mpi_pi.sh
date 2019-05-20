#!/bin/bash 

cd HPC_exercise/Parallel_computing/ex_02
module load openmpi

mpicc mpi_pi.c -std=gnu99  -o ex


if [ -f openmpi.txt ]; then rm openmpi.txt; fi
echo '-----TIME--- ' >> openmpi.txt
for i in 1 2 4 8 12 16 20 24 28 32 36 40; do
	mpirun -np ${i} ./ex >> openmpi.txt
	done







