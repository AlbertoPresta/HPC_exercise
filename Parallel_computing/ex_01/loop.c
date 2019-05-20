#include<stdlib.h>
#include<stdio.h>
#include<omp.h>



void print_usage(int* a ,int N,int nthreads){
        int tid,i;
        for(tid=0;tid<nthreads;++tid){
           fprintf(stdout,"%d:  ",tid);
           for(i =0;i<N;++i){
              if(a[i]==tid) fprintf(stdout,"*");
              else fprintf(stdout," ");
            }
            printf("\n");
        }
}

int main(int argc,char * argv[]){
    const int N = 100;
    int a[N];
    int thread_id = 0;
    int nthreads = 1;
    int i = 0;

    #pragma omp parallel private(thread_id,i)
    {
       int nthreads = omp_get_num_threads(); // return number of threads
       thread_id = omp_get_thread_num(); // return the id threads
	// in the following frame of code, We will call the function
	//print_usage() (which simply a "*" in a multhi-thread world) using 
	//different way of scheduling threads job
       #pragma omp for  schedule(static)
           for( i = 0;i<N;i++){a[i]=thread_id;}
           #pragma omp single
           {
              printf("static\n");
              print_usage(a,N,nthreads);
            }
           #pragma omp for  schedule(static,1)
             for( i = 0;i<N;i++){a[i]=thread_id;}
             #pragma omp single
             {
                printf("static, with chunck_size=1\n");
                print_usage(a,N,nthreads);
             }
          #pragma omp for  schedule(static,10)
             for( i = 0;i<N;i++){a[i]=thread_id;}
             #pragma omp single
             {
 				printf("static, with chunck_size=10\n");
 			    print_usage(a,N,nthreads);
              }
           #pragma omp for  schedule(dynamic,1)
              for( i = 0;i<N;i++){a[i]=thread_id;}
              #pragma omp single
              {
     			 printf("dynamic");
     			 print_usage(a,N,nthreads);
               }    
			#pragma omp for  schedule(dynamic,10)
 			  	for( i = 0;i<N;i++){a[i]=thread_id;}
   				#pragma omp single
      		  	{
      			   printf("dynamic,10");
     			   print_usage(a,N,nthreads);
                }    
     }
    return 0;
}
