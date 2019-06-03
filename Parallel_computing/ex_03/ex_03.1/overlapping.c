include<mpi.h>
#include<stdlib.h>
#include<stdio.h>



// function which prints portion of the matrix

void print(int* local_mat,int n_loc,int N,FILE* file){
        for(int j = 0; j<n_loc;j++){
                for(int k =0;k<N;k++){fprintf(file,"%d ",local_mat[j*N + k]);}
                fprintf(file,"\n");
                }
        }

int main(int argc,char* argv []){
        int N = 50;
        int npes = 1;
        int rank=0;
        FILE *file = fopen("mpi_overlapping_id.txt","w");
        fprintf(file,"IDENTITY MATRIX OF DIMENSION %d\n",N);
        MPI_Init(&argc,&argv);
              MPI_Comm_rank(MPI_COMM_WORLD,&rank);
              MPI_Comm_size(MPI_COMM_WORLD,&npes);
              int rest = N%npes;
              int n_loc = N/npes;
              if(rank<rest){n_loc=n_loc+1;}
              int local_start;
              if(rank<rest){local_start=rank*n_loc;}
              else{local_start = rest + rank*n_loc;}
              int* local_mat = (int*)malloc(N*n_loc*sizeof(int));
               for(int i=0;i<n_loc;i++){local_mat[(N+1)*i+local_start]=1;}
              if(rank!=0){
                MPI_Request req;
                MPI_Isend(local_mat,N*n_loc,MPI_INT,0,101,MPI_COMM_WORLD,&req);
                 }
              if(rank==0){
                 MPI_Request req;
                 int* new_local_mat = (int*)malloc(N*n_loc*sizeof(int));
                 for(int i=1;i<npes;i++){
                     if(rest!=0 && i==rest){n_loc=n_loc-1;} 
                     MPI_Irecv(new_local_mat,N*n_loc,MPI_INT,i,101,MPI_COMM_WORLD,&req);
                     // meanwhile procs is receiving message, it prints local mat to exploit non blocking communication
                     print(local_mat,n_loc,N,file);
                     MPI_Wait(&req, MPI_STATUS_IGNORE);
                     //  then we copy new_local_mat (procs receives it) in local mat.
                     int* temp;
                     temp = new_local_mat;
                     new_local_mat = local_mat;
                     local_mat = temp;
                   }
				   // as we are in rank 0 we print local_mat (just become new_l$
				                      print(local_mat,n_loc,N,file);
				                   free(new_local_mat);
				                   }
				               free(local_mat);
				               MPI_Finalize();
				               return 0;
				               }
				   
