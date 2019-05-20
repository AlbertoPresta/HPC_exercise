#!/bin/bash 

cd HPC_exercise/Parallel_computing/ex_03/ex_03.2

module load openmpi

mpicc message_ring.c -std=gnu99 -o message
mpicc ring.c -std=gnu99 -o ring

if [-f message_ring.txt ]; then rm message_ring.txt; fi
echo "number of processes: $p">>message_ring.txt
mpirun -np 4  message >> message_ring.txt
echo " " >>message_ring.txt
	
if [-f ring.txt ]; then rm ring.txt; fi
echo "number of processes: $p">>ring.txt
mpirun -np 4 ring >> ring.txt
echo " " >>ring.txt
                                
	
