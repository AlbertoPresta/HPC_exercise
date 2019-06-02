#include<stdio.h>
#include<stdlib.h>

#define N 100
#define THREADS_PER_BLOCKS 10

//space for functions

__global__ void reverseArrayBlock(int *dev_a,int *dev_b){
        int bx = blockIdx.x;
        int tx = threadIdx.x;
        int old_id = blockDim.x*bx + tx;
        int new_id = (blockDim.x*gridDim.x) - 1 - old_id;

        dev_b[old_id] = dev_a[new_id];
}





int main(void){

        //pointer for host memory
        int *h_a;
        //pointer for device memory
        int *dev_a;
        int *dev_b;

        // compute number of blocks needed
        int numblocks = N/THREADS_PER_BLOCKS;

        //allocate host and device memory
        int mem_size = N*(sizeof(int));
        h_a = (int*)malloc(mem_size);
        cudaMalloc((void**)&dev_a,mem_size);
        cudaMalloc((void**)&dev_b,mem_size);
        //initialize array
        for (int i = 0;i<N;i++){h_a[i]=i;}

        // copy host array yo device array (in order to launch kernel)

        cudaMemcpy(dev_a,h_a,mem_size,cudaMemcpyHostToDevice);


        //launch kernel

        reverseArrayBlock<<<N/THREADS_PER_BLOCKS,THREADS_PER_BLOCKS>>>(dev_a,dev_b);


        //device to host copy
		cudaMemcpy(h_a,dev_b,mem_size,cudaMemcpyDeviceToHost);

		        for (int i =0; i<N;i++){printf("%d ",h_a[i]);}
		        // free davice memory
		        cudaFree(dev_a);
		        cudaFree(dev_b);

		        //free host_memory
		        free(h_a);
		        return 0;
		}
		