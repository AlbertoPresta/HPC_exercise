#!/bin/bash

cd /home/apresta/second_semester/HPC_exercise/Parallel_computing/gpu_ex
module load cudatoolkit/10.0


printf "\n\n\n" >> results.txt
echo "512 thread_per_blocks" >> results.txt
./512_shared_transpose >> results.txt

printf "\n\n\n" >> results.txt
echo "1024 thread_per_blocks" >> results.txt
./1024_shared_transpose >> results.txt


printf "\n\n\n" >> results.txt
echo "64 thread_per_blocks" >> results.txt
./64_shared_transpose >> results.txt


printf "\n\n\n" >> results.txt
echo "64 thread_per_blocks" >> results.txt
./64_shared_transpose >> results.txt

printf "\n\n\n" >> results.txt
echo " naive" >> results.txt
./transpose >> results.txt

rm test.sh.*
exit
