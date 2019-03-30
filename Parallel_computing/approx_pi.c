#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


double approx_pi(int N){
    double h =(1.0/N);
    double pi = 0.0;
    int i =0;
    for (i=0;i<N;i++){
       double  x_i=h*i; // passo
       double  f_i=1.0/(1+x_i*x_i);
       pi += f_i; 
      }
   pi = 4*h*pi;
   return pi;
}








int main(){
double pi = approx_pi(100000000);
printf("%f\n",pi);

return 0;
}