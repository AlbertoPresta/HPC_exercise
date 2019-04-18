 #include<mpi.h>
#include<stdlib.h>
#include<stdio.h>





int main(int argc, char * argv[]){
	int npes = 1;
	int rank = 0;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&npes);
	int sum = 0;
	int local = rank;
	for(size_t i  = 0;i<npes; i++ ){
	MPI_Status status;
	MPI_Request req;
	MPI_Isend(&local,1,MPI_INT,(rank+1)%npes,101,MPI_COMM_WORLD,&req);
	MPI_Irecv(&local,1,MPI_INT,(rank - 1 + npes)%npes,101,MPI_COMM_WORLD,&req);
	MPI_Wait(&req, &status);
	sum = sum + local;
	}
	printf("%d\n",sum);
	MPI_Finalize();
	return 0;
 }














