#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define N 8192
#define N_THREADS 64

// space for function

__global__ void mat_transpose(int *mat_in_dev, int *mat_out_dev){
        int index = threadIdx.x  +  blockIdx.x*blockDim.x;
        int x = index%N;
        int y = index/N;
        mat_out_dev[y*N+ x] = mat_in_dev[x*N+y];
        }

void fill_matrix(int *p){
        for(int i = 0; i<N;i++){
                for(int j = 0; j<N;j++){p[i*N + j] = rand()%100 + 1;}
                }
        }

void print_matrix(int *p){
        for(int i = 0;i<N;i++){
                for(int j=0;j<N;j++){printf("%d ",p[i*N + j]);}
                printf("\n");
}
}


int main(void){

        int *mat_in_h;
        int *mat_out_h;
        int *mat_in_dev;
        int *mat_out_dev;
		
		
		// request of host memory

		 mat_in_h = (int *)malloc(N*N*sizeof(int));
		 mat_out_h = (int *)malloc(N*N*sizeof(int));

		 // request of device memory

		 cudaMalloc((void**)&mat_in_dev,N*N*sizeof(int));
		 cudaMalloc((void**)&mat_out_dev,N*N*sizeof(int));

		 // inizialization of the matrix
		 fill_matrix(mat_in_h);
		 int size = N*N*sizeof(int);
		 // copy from host memomy to device memory

	         cudaMemcpy(mat_in_dev,mat_in_h,size,cudaMemcpyHostToDevice);


		cudaEvent_t start, stop;
	  	cudaEventCreate(&start);
		cudaEventCreate(&stop);

		
	      // kernel
		int N_BLOCKS = N*N/N_THREADS;
		cudaEventRecord(start);                
                mat_transpose<<<N_BLOCKS,N_THREADS>>>(mat_in_dev,mat_out_dev);
		cudaEventRecord(stop);
		//copy from device memory to host memory

		cudaMemcpy(mat_out_h,mat_out_dev,size,cudaMemcpyDeviceToHost);
				       
		cudaEventSynchronize(stop);
		float milliseconds = 0;
		cudaEventElapsedTime(&milliseconds, start, stop);
		printf("dimensions of block: %d x %d\n",8,8);
        	printf(" Time in milliseconds: %f\n",milliseconds);
        	printf("Bandwidth: %f GB/s\n",2*size/milliseconds/1e6);		
		
		
						       
		    // free the memory

		free(mat_in_h);
		free(mat_out_h);
	        cudaFree(mat_in_dev);
		cudaFree(mat_out_dev);
		return 0;

	}

			
