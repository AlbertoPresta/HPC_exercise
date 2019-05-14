#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 8192
#define LINEAR_SIDE 32

void print_matrix(int *p){
        for(int i = 0;i<N;i++){
                for(int j=0;j<N;j++){printf("%d ",p[i*N + j]);}
                printf("\n");
}
}


void fill_matrix(int *p){
        for(int i = 0; i<N;i++){
           for(int j = 0; j<N;j++){p[i*N + j] = rand()%100 + 1;}
           }
 }

bool verify(int *a,int*b){
	
	for(int i =0;i<N*N;i++){if(a[i]!=b[(i%N)*N + i/N]) return 0;}
	return 1;	
}


__global__ void transpose(int *mat_in_dev, int *mat_out_dev){

	//local memory location where to save  matrix portion
	//related to the blocks
	// shared by all threads within a block
	__shared__  int temp_matrix[LINEAR_SIDE][LINEAR_SIDE];
	
//	creation of the global indexes in order to journey to the matrix 	
	int global_x = blockIdx.x*blockDim.x + threadIdx.x;
	int global_y = blockIdx.y*blockDim.y + threadIdx.y;
	
//	now we created global indexes referred to transpose matrix

	//copy in the sub matrix
	temp_matrix[threadIdx.x][threadIdx.y] = mat_in_dev[global_x*N + global_y];
	
	__syncthreads();
	
	//copy submatrix in out
	
	mat_out_dev[global_y*N + global_x]=temp_matrix[threadIdx.x][threadIdx.y];
	
}



                                                
int main(void){
	
	int *mat_in_h;
	int *mat_out_h;
	int *mat_in_dev;
	int *mat_out_dev;

	//allocation of memory
	mat_in_h = (int*)malloc(N*N*sizeof(int));
	mat_out_h = (int*)malloc(N*N*sizeof(int));
	cudaMalloc((void**)&mat_in_dev,N*N*sizeof(int));
	cudaMalloc((void**)&mat_out_dev,N*N*sizeof(int));
	fill_matrix(mat_in_h);	

	//copy matrix from host to device
	int size = N*N*sizeof(int);
	cudaMemcpy(mat_in_dev,mat_in_h,size,cudaMemcpyHostToDevice);
	
	// definition of variables dim3
	dim3 grid,block;
	
	block.x = LINEAR_SIDE;
	block.y = LINEAR_SIDE;
	grid.x = N/block.x;
	grid.y = N/block.y;

//	timing
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);



	cudaEventRecord(start);
	transpose<<<grid,block>>>(mat_in_dev,mat_out_dev);
	cudaEventRecord(stop);	

	// copy from device to host
	
	cudaMemcpy(mat_out_h,mat_out_dev,size,cudaMemcpyDeviceToHost); 
	printf("%d\n",verify(mat_in_h,mat_out_h));

	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("dimensions of block: %d x %d\n",LINEAR_SIDE,LINEAR_SIDE);
        printf(" Time in milliseconds: %f\n",milliseconds);
        printf("Bandwidth: %f GB/s\n",2*size/milliseconds/1e6);


	//free the memory
	free(mat_in_h);
	free(mat_out_h);
	cudaFree(mat_in_dev);
	cudaFree(mat_out_dev);
	
	
	return 0;
}
