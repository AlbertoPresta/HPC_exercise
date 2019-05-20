#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>



int main(int argc, char* argv[]){
    int npes = 1;
    int rank = 0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&npes);
    size_t N =5;
    int* sum = (int*)malloc(N*sizeof(int)); // allocation of memory for the entire sum
    int* local = (int*)malloc(N*sizeof(int)); // allocation of message to be sent in the ring
    for(size_t i = 0; i<N;i++){local[i]= i;}
    // ring message passing
    for (size_t i = 0; i<npes; i++){
    //creation of the variable req, for MPI_WAIT
    MPI_Request req;
    MPI_Isend(local,N,MPI_INT,(rank+1)%npes,101,MPI_COMM_WORLD,&req); // sending npes time
    for(size_t i = 0; i<N;i++){sum[i] = sum[i] + local[i];}
   MPI_Wait(&req, MPI_STATUS_IGNORE);
    MPI_Irecv(local,N,MPI_INT,(rank - 1 + npes)%npes,101,MPI_COMM_WORLD,&req);
    }
    for (size_t i = 0; i <N; i++){printf("%d ", sum[i]); }
    printf(" from proc %d\n", rank);
     MPI_Finalize();
    return 0;
}
