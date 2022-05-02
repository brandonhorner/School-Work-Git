/*
Brandon Kealii Horner
9/27/2019
Assignment 5: Question 1
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate openMP code. This file can be run like so:
        (in the terminal window in the current directory)
        gcc crazy_scientist_v1_bkh76.c -o -fopenmp crazy_scientist_v1_bkh76 -lm

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
  
  #pragma omp parallel for private(i, j) shared(mat) schedule(static, SIZE/2)
  //modify code here
  for (i=0;i<SIZE;i++) { /* loop over the rows */
    for (j=0;j<SIZE;j++) {  /* loop over the columns */
      mat[i][j] = do_crazy_computation(i,j);
      fprintf(stderr,".");
    }
  }
  double tend=omp_get_wtime();
  double elapsed=tend - tstart;
  printf("Elapsed time: %f seconds\n",elapsed);

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
