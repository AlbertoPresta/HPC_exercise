cd second_semester/day2


 gcc -fopenmp loop.c -std=gnu99 -o loop


if [ -f loop.txt ]; then rm loop.txt; fi
export OMP_NUM_THREADS=10
echo "Run loop.c with 10 threads\n"
echo "                           "
./loop >> loop.txt



exit
