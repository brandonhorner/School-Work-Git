/*
Brandon Kealii Horner
9/26/2019
Assignment 3: Question 3
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate thread code. This file can be run with the A3orderexec_test.py
  file provided, like so:
        (in the terminal window in the current directory)
        gcc .q3.c -lpthreads -o .q3
        ./q3 | python A3orderexec_test.py
*/
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 10

struct arguments{
    // value will be the programmer-assigned ID for each thread
    int value;
    // shared counter
    int *total_count;
    pthread_mutex_t *mutex;
};

void *do_work(void *arg);


// "index" of which thread goes next
int working_thread_index = 0;

// this flag will be false when the total_count is 990
bool flag = true; 

pthread_barrier_t barrier;

int main(int argc, char *argv){
    // declare the worker_threads as an array of threads
    pthread_t worker_thread[ NUM_THREADS ];

    // declare and initialize the mutual exclusion lock
    pthread_mutex_t lock;
    pthread_mutex_init( &lock, NULL );
    
    // declare an array of arguments for the threads
    struct arguments *arg[ NUM_THREADS ];
    
    // this is the count the is added to by each thread (in order)
    int total_count = 0;
    
    // Build arguments to threads
    int thread_num;
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ )
    {
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->value = thread_num;
        arg[ thread_num ]->total_count = &total_count;
        arg[ thread_num ]->mutex = &lock;
    }
    
    // Barrier initialization
    if( pthread_barrier_init( &barrier, NULL, NUM_THREADS ) ){
        printf( "Could not create a barrier\n" );
        return -1;
    }
    
    // Create 10 threads calling do_work()
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ ){

        if( pthread_create( &worker_thread[ thread_num ], NULL,
                        do_work, ( void * )arg[ thread_num ] ) ){
            fprintf ( stderr,"Error while creating thread %d\n", thread_num );
            exit(1);
        }
    }
    
    // Join all 10 threads with main thread
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ ){
        if( pthread_join( worker_thread[ thread_num ], NULL ) ){
            fprintf ( stderr,"Error while joining with child thread %d\n", thread_num );
            exit(1);
        }    
    }
    
    pthread_mutex_destroy( &lock );
    printf( "Total: %d\n", total_count );
    
    exit(0);
}

/* The do_work() function inserts the threads value into a shared buffer. Once all of 
    the threads have finished, they wait at the barrier for each other. Then, one non-
    deterministically chosen thread will continue and check the buffer for the
    correct output sequence: 1, 2, 3. If the sequence is correct, we increase the
    count of correct sequences until we hit 10 correct sequences. In other words,
    the three threads will fill the buffer many times and output how many times
    it took for the order to be correct 10 times.
    
    Parameters: arg - the arguments passed into the threads.
*/
void *do_work( void *arg ){
    // Pass in arguments
    struct arguments *argument;
    argument = ( struct arguments* )arg;  
    int thread_id = argument->value;
    int *total_count = argument->total_count;
    pthread_mutex_t *mutex = argument->mutex;
    
    while( flag ){
        //printf("thread_%d entering loop\n", thread_id);
        if( thread_id == working_thread_index ){
            // lock the critical section
            pthread_mutex_lock( mutex );

            working_thread_index ++;
            // if we are at the last thread, reset the working_thread_index
            if( thread_id == NUM_THREADS - 1 )
            {
                working_thread_index = 0;
            }
            if( *total_count == 990 )
            {
                flag = false;
                pthread_mutex_unlock( mutex );
                return NULL;
            }
            // add the value of the thread to the total_count
            *total_count += thread_id;
            printf( "my num: %d, total: %d\n", thread_id, *total_count);

            // release the mutual exclusion lock
            pthread_mutex_unlock( mutex );        
        }
    }
    return NULL;
}

