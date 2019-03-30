cd second_semester/day2

gcc -fopenmp parallel_pi.c -o parallel -std=gnu99

if [ -f parallel.txt ]; then rm parallel.txt; fi
echo '-----PARALLEL PI--- ' >> parallel.txt
for thread in 1 2 4 8 16 20
        do
           echo '----------------------'>> parallel.txt
           export OMP_NUM_THREADS=${thread}
           echo 'Number of thread(s):'${thread} >> parallel.txt
           ./parallel >> parallel.txt
           echo '----------------------'>> parallel.txt
        done