#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

// in this code there are three different function 
// which calculate pi using an openmp approach and 
// using three different commands
double critical_parallel_approx_pi(size_t N){
   //define values useful for calculationg Pi 
   double h =(1.0/N); // step 
   double pi = 0.0; // global Pi
   #pragma omp parallel
   {
     double local_res = 0.0; // local_result 
     #pragma omp for schedule(static) // schedule jobs
        for (size_t i=0;i<N ;++i){
           double x_i = (i+0.5)*h;
           local_res += 1.0/(1.0 + x_i*x_i);
          }
      #pragma omp critical // using critical to avoid race condition
      {
         pi += local_res;
       }
    }
  return 4*h*pi;
  }

double atomic_parallel_approx_pi(size_t N){
        double pi = 0.0;
        double h = (1.0/N);
        #pragma omp parallel
        {
	       double local_sum = 0.0;
           #pragma omp for schedule(static)
               for(size_t i=0; i<N;i++){
                  double x_i = (i+0.5)*h;
                  local_sum += 1.0/(1.0 + x_i*x_i);
                  }
	       #pragma omp atomic      
           pi += local_sum;
	     }
      	return 4*pi*h;
}
double reduction_parallel_approx_pi(size_t N){
    double pi =0.0;
    double h = 1.0/(double)N;
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
       for (size_t i=0; i<N;i++){
           double x_i = (i+0.5)*h;
           sum += 4.0/(1.0 + x_i*x_i);
           }
	pi = h*sum;
    return pi;
}




int main(){
double start_1,end_1,start_2,end_2,start_3,end_3;
printf("--------TIMING--------\n");
printf("critical         atomic         reduction\n");
start_1 = omp_get_wtime();
double pi = critical_parallel_approx_pi(1000000000);
end_1 = omp_get_wtime() - start_1;

start_2 = omp_get_wtime();
double pi_2 = atomic_parallel_approx_pi(1000000000);
end_2 = omp_get_wtime() - start_2;

start_3 = omp_get_wtime();
double pi_3 = reduction_parallel_approx_pi(1000000000);
end_3 = omp_get_wtime() - start_3;
printf("%f         %f         %f",end_1,end_2,end_3);
return 0;

}
