#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>


/*
In this Exercise I want to define the NxN identity matrix
Every process will have saved a local_iXN memory, where local_i 
represents the local indexes
*/



//function for printing portion of matrix stored in each process

void print(int* local_mat,int n_loc,int N,FILE* file){
	for(int j = 0; j<n_loc;j++){
		for(int k =0;k<N;k++){fprintf(file,"%d ",local_mat[j*N + k]);}
	        fprintf(file,"\n");
	        }
    }



int main(int argc,char* argv[]){
	int N = 50; //dimension of the matrix
	int npes=1;
	int rank=0;
	FILE *file = fopen("mpi_identity.txt", "w");
	fprintf(file,"IDENTITY MATRIX OF DIMENSION %d\n",N);
	MPI_Init(&argc,&argv);
		MPI_Comm_rank(MPI_COMM_WORLD,&rank);
		MPI_Comm_size(MPI_COMM_WORLD,&npes);
		int rest = N%npes;
		int n_loc = N/npes;
		if(rank<rest){n_loc = n_loc +1;}
		int local_start;
		//initialization of local_start: 
		//for processes which are greater than the rest, we don't add any loc, but in order to reach them we 
		//have to add the rest (indeed the rest has been divided through the previous processes)
		if(rank<rest) {local_start= rank*(n_loc);}
    		else {local_start= rest + (rank*n_loc);} 
		//allocate space for local domain in every process:every process has a portion of the entire matrix, 
		//which phsically doesn't exist.
		int* local_mat = (int*)malloc(N*n_loc*sizeof(int));
		//inizialization of each sub-matrix in every process
		for (int i = 0; i <n_loc; i++){local_mat[(N+1)*i+local_start]=1;}
		//try to print the matrix:
		// Obviously we can't simply print the matrix (ideed processes work in parallel, so the output would not be the 
		//identity matrix)
		// We should sequentially send to P0 protion of the matrix through blocking communication
		if(rank!=0){MPI_Send(local_mat,N*n_loc,MPI_INT,0,101,MPI_COMM_WORLD);}
		if(rank==0){
		 print(local_mat,n_loc,N,file);
		 //receive portion of matrix from other processes
		 int* new_local_mat = (int*)malloc(N*n_loc*sizeof(int)); // this will be reserved for receiving submatrix
		 // now i pass thorough different processes
		 for(int i =1; i<npes;i++){
		 	if(rest!=0 && i==rest){n_loc = n_loc -1;}//change value of n_loc
		 	MPI_Recv(new_local_mat,N*n_loc,MPI_INT,i,101,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		 	print(new_local_mat,n_loc,N,file);
                  	}
                 free(new_local_mat);
                  }
                free(local_mat);

	MPI_Finalize();
	fclose(file); 
	return 0;
	}

