/*
Brandon Kealii Horner
11/26/2019
Assignment 6: Question 2


70% of this code was boilerplate provided by Prof. Michael Gowanlock at NAU.
 I used the qsort(), merging algorithms and cmpfunc() from the powerpoint
 presentations provided and modified them for the problem. All credit goes
 to Mike.


This program speeds up the execution time of question 1. First the program
  generates an array of N points (with an x and a y coordinate). It then uses
  merge and quicksort to sort the points by x-coordinate (sorting low to high).
  Once sorted, the same O(n^2) algorithm from question 1 calculates each
	points' distance against all other points. However, this new algorithm will
	first check to see if the point is about to calculate a distance against
	itself. Then if there are any points farther than epsilon in the x direction
	those points will be ignored from the calculation process. By doing this the
	algorithm avoids expensive calculations. Furthermore the algorithm uses static
	scheduling with a chunk size of epsilon * 2 to split up the x coordinates
	by even spaces. Finally, this algorithm differs from question 1 because it
	does not take the square root in the euclidean distance, and instead squares
	epsilon.


Example compilation command:
		gcc -fopenmp question2_bkh76.c -fopenmp question2_bkh76 -lm
to run the program:
		./question2_bkh76 5.0
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


int idMin(double * arr, int size);
void generateDataset(struct pointData * data);


int cmpfunc(const void *x, const void *y) {
  if (*(double*)x > *(double*)y) return 1;
  if (*(double*)x < *(double*)y) return -1;
  return 0;
}


int main(int argc, char *argv[])
{
	//Read epsilon distance from command line
	if (argc!=2)
	{
  	printf("\nIncorrect number of input parameters. Please input an epsilon distance.\n");
  	return 0;
	}

	char inputEpsilon[20];
	strcpy(inputEpsilon,argv[1]);
	double epsilon=atof(inputEpsilon);
  double generation_start=omp_get_wtime();

	//generate dataset:
	struct pointData *data;
	data = (struct pointData*)malloc(sizeof(struct pointData)*N);
	printf("\nSize of dataset (MiB): %f",(2.0*sizeof(double)*N*1.0)/(1024.0*1024.0));
	generateDataset(data);

	//change OpenMP settings:
	omp_set_num_threads(NUM_THREADS);

	//begin merge/quick sort on x coordinates
  //create array for later merging
	struct pointData * mergedData;
	mergedData=(struct pointData *)malloc(N * sizeof(struct pointData));

	int chunksize = N/NUM_THREADS;

	//first: break the original array into multiple sub arrays
  struct pointData ** dataSubArrays;
  dataSubArrays= (struct pointData**)malloc(NUM_THREADS * sizeof(struct pointData*));
	int i;
  for (i=0; i<NUM_THREADS; i++){
	  //allocate memory for subarray
	  dataSubArrays[i]=(struct pointData*)malloc(chunksize * sizeof(struct pointData));
	  //starting indexes of subarrays for memcpy
	  int start=i*chunksize;
	  memcpy(dataSubArrays[i], data+start, chunksize * sizeof(struct pointData));
  }

	//perform a quicksort on each sub array
	for (i=0; i<NUM_THREADS; i++){
		qsort(dataSubArrays[i], chunksize, sizeof(struct pointData), cmpfunc);
	}

	//merge subchunks:
	int nextPtr[NUM_THREADS]; //next ptr
	double tmpArr[NUM_THREADS]; //values used to find the minimum
	int countNum[NUM_THREADS]; //number of values "selected" from a subarray
													//corner case: don't want this to exceed the chunksize
	int j=0;
	int idInc=0;

	//init
	for (i=0; i<NUM_THREADS; i++){
		nextPtr[i]=0; //index into the subarrays
		tmpArr[i]=0.0;
		countNum[i]=0;
	}

	for (i=0; i<N; i++){
		//fill tmp array with the next elements from each array
		//corner case: put large value in if the subarray is done
		for (j=0; j<NUM_THREADS; j++){
			//add to the array the next value
			if (countNum[j]<chunksize){
				tmpArr[j]=dataSubArrays[j][nextPtr[j]].x;
			}
			//if the chunk is done:
			else{
				tmpArr[j]=RAND_MAX;
			}
		}
    //compare:
    idInc=idMin(tmpArr,NUM_THREADS);
    //updated merged array
    mergedData[i]=dataSubArrays[idInc][nextPtr[idInc]];
    //update the next ptr
    nextPtr[idInc]++;
    //update the number of values from the sub array
    countNum[idInc]++;

  } //end for
	// end of sorting

	double computation_start=omp_get_wtime();
	fprintf(stdout, "\nTotal time generating and sorting (s): %f\n",
										computation_start - generation_start);

  int count = 0;
  double distance = 0;

	int chunk = (int)epsilon*2;
	#pragma omp parallel for private(i, j, distance) shared(mergedData, epsilon) schedule(static, chunk) reduction(+:count)
	for (i=0; i<N; i++){
			for (j=0; j<N; j++){
				if (i == j){
					count++;
				}
				//see if the points are already too far away to consider checking
				else if((mergedData[j].x - mergedData[i].x) > epsilon){
					// do nothing
				}
				else{
					// calculate the euclidean distance (without taking square root)
					distance = ((mergedData[i].x - mergedData[j].x)
															* (mergedData[i].x - mergedData[j].x))
																+ ((mergedData[i].y - mergedData[j].y)
																		* (mergedData[i].y - mergedData[j].y));
 					//instead of taking square above, we can square epsilon
					if (distance <= epsilon*epsilon)
						count++;
				}
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
	double max_x = 0.0;
	double max_y = 0.0;
	for (unsigned int i=0; i<N; i++)
  {
		data[i].x=1000.0*((double)(rand()) / RAND_MAX);
		data[i].y=1000.0*((double)(rand()) / RAND_MAX);
	}
}

//returns the id in the array that contains the smallest element
int idMin(double * arr, int size){
	int i=0;
	double smallest=RAND_MAX;
	int idx=0;

	  for(i=0; i<size; i++){
	    if (arr[i]<smallest){
	      smallest=arr[i];
	      idx=i;
	    }
	  }
	return idx;
}
