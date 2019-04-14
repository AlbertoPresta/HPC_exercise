 #include<stdlib.h>
 #include<stdio.h>
 #include<mpi.h>
 



double mpi_pi(double local_a,double local_b,double h){
   double local_res = 0.0;
   while(local_a<=local_b){
   	double x_i = local_a + 0.5*h;
   	local_res += 1.0/(1.0 + x_i*x_i);
   	local_a = local_a + h;
   	}
    local_res = 4*h*local_res;     
    return local_res;
    }



 int main(int argc, char * argv[]){
	int N = 1000000000;
	double h = 1.0/N;
	int rank = 0;// store the MPI identifier of the process
	int npes = 1; // store the number of MPI processes
	double inizio = MPI_Wtime();
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &npes );
	int count = N/npes;
	double start = rank*count*h;
	double end= start + count*h;
	double local = mpi_pi(start,end,h);
	double global_res;
	MPI_Reduce(&local,&global_res,1,MPI_DOUBLE,MPI_SUM,npes-1,MPI_COMM_WORLD);
	double fine = MPI_Wtime();
	if(rank==npes-1){MPI_Send(&global_res,1,MPI_DOUBLE,0,101,MPI_COMM_WORLD);}
	if(rank==0){
	MPI_Recv(&global_res,1,MPI_DOUBLE,npes-1,101,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	printf("%f\n", fine - inizio);
	}
	MPI_Finalize();

	return 0;
	
}

