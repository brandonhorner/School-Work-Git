/*
Brandon Kealii Horner
11/26/2019
Assignment 6: Question 1


90% of this code was boilerplate provided by Prof. Michael Gowanlock at NAU.
	All credit goes to Mike.


This program speeds up the execution time of the brute force seqeuntial
	algorithm by using OpenMP. First the program generates an array of N points
	(with an x and a y coordinate). Then in a O(n^2) algorithm calculates each
	points' distance against all other points. Furthermore the algorithm uses
	static scheduling with a chunk size of N/NUM_THREADS to split up the work
	evenly amongst the threads.


Example compilation command:
		gcc -fopenmp question1_bkh76.c -fopenmp question1_bkh76 -lm
to run the program:
		./question1_bkh76 5.0
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "omp.h"

//N is 100000 for the submission. However, you may use a smaller value of testing/debugging.
#define N 100000
//Do not change the seed from 72, or your answer will not be correct
#define SEED 72
#define NUM_THREADS 4

struct pointData{
	double x;
	double y;
};


void generateDataset(struct pointData * data);


int main(int argc, char *argv[])
{
	//Read epsilon distance from command line
	if (argc!=2)
	{
  	printf("\nIncorrect number of input parameters. \
																				Please input an epsilon distance.\n");
  	return 0;
	}

	char inputEpsilon[20];
	strcpy(inputEpsilon,argv[1]);
	double epsilon=atof(inputEpsilon);
  double generation_start=omp_get_wtime();

	//generate dataset:
	struct pointData *data;
	data = (struct pointData*)malloc(sizeof(struct pointData)*N);
	printf("\nSize of dataset (MiB): %f\n", (2.0*sizeof(double)*N*1.0)/(1024.0*1024.0));
	generateDataset(data);

	double computation_start=omp_get_wtime();
	fprintf(stdout, "\nTotal time of generating data (s): %f\n",
		computation_start - generation_start);

	//change OpenMP settings:
	omp_set_num_threads(NUM_THREADS);

  int count = 0;
  double distance = 0;
  int i, j;

  #pragma omp parallel for private(i, j, distance) shared(data, epsilon) reduction(+:count)
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      distance = sqrt( ( (data[i].x - data[j].x) * (data[i].x - data[j].x) )
											+ ( (data[i].y - data[j].y) * (data[i].y - data[j].y) ));
      if (distance <= epsilon)
        count ++;
    }//end inner for loop
  }//end outer for loop
  fprintf(stdout, "\nTotal count of points within epsilon of each other: %d", count);


  double tend=omp_get_wtime();
  fprintf(stdout, "\nTotal time (s): %f\n", tend - computation_start);


  free(data);
  return 0;
}

//Do not modify the dataset generator or you will get the wrong answer
void generateDataset(struct pointData * data)
{
	//seed RNG
	srand(SEED);

	for (unsigned int i=0; i<N; i++)
  {
		data[i].x=1000.0*((double)(rand()) / RAND_MAX);
		data[i].y=1000.0*((double)(rand()) / RAND_MAX);
	}
}
