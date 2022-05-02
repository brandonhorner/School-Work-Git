/*
Brandon Kealii Horner
9/27/2019
Assignment 5: Question 3
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate openMP code. This file can be run like so:
        (in the terminal window in the current directory)
        gcc crazy_scientist_v3_bkh76.c -o -fopenmp crazy_scientist_v3_bkh76 -lm

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

#define SIZE 50

double do_crazy_computation(int i,int j);

int main(int argc, char **argv) {
  double mat[SIZE][SIZE];
  int i,j;
  omp_set_num_threads(2);
  double tstart=omp_get_wtime();
  double thread_numbers[2];
  double thread_times[2] = {0.0, 0.0};
  double time_interval_start;  
  double time_interval_end;  
  double time_interval;  
  
  #pragma omp parallel private(i, j) shared(mat, thread_times)
  {
    #pragma omp single
    {
      for (i=0;i<SIZE;i++) { /* loop over the rows */
        #pragma omp task
        {
          for (j=0;j<SIZE;j++) {  /* loop over the columns */
            time_interval_start=omp_get_wtime();
            mat[i][j] = do_crazy_computation(i,j);
            fprintf(stderr,".");
            time_interval_end=omp_get_wtime();
            time_interval = time_interval_end - time_interval_start;
            //if the thread is the thread defined in single
            if( omp_get_thread_num() == 0 )
              thread_times[0] += time_interval; 
            //otherwise we have the other thread
            else
              thread_times[1] += time_interval;
          }//end inner for
        }//end task section
      }//end outter for
    }//end single
  }//end parallel
  printf("\nthread1=%f", thread_times[0]);
  printf("\nthread2=%f", thread_times[1]);
  printf("\nload_imbalance=%f", fabs(thread_times[0] - thread_times[1]));
  double tend=omp_get_wtime();
  double elapsed=tend - tstart;
  printf("\nElapsed time: %f seconds\n",elapsed);

  exit(0);
}

//Crazy computation
double do_crazy_computation(int x,int y) {
   int iter;
   double value=0.0;

   for (iter = 0; iter < 5*x*x*x+1 + y*y*y+1; iter++) {
     value +=  (cos(x*value) + sin(y*value));
   }
  return value;
}