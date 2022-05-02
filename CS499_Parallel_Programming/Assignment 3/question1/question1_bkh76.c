/*
Brandon Kealii Horner
9/24/2019
Assignment 3: Question 1
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate thread code. This file can be run with the A3sequence_test.py
  file provided, like so:
        (in the terminal window in the current directory)
        gcc .q1.c -lpthreads -o .q1
        ./q1 | python A3sequence_test.py
*/
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 3

struct arguments {
    int val;
    int *correct_count;
    int *total_count;
    pthread_mutex_t *mutex;
};

void *do_work(void *arg);

bool buffer_is_correct();
bool buffer_is_full();

// globally shared buffer
int buffer[ NUM_THREADS ];

// index of the buffer to be added to
int working_index = 0;

// a flag to see if a thread has already checked the buffer contents
bool checked = false;


pthread_barrier_t barrier;

int main(int argc, char *argv){
    pthread_t worker_thread1;
    pthread_t worker_thread2;
    pthread_t worker_thread3;
    pthread_mutex_t lock;
    pthread_mutex_init( &lock, NULL );
    
    struct arguments *arg[ NUM_THREADS ];
    
    int correct_count = 0;
    int total_count = 0;
    
    int thread_num;
    
    // Build arguments to threads
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ )
    {
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->val = thread_num + 1;
        arg[ thread_num ]->correct_count = &correct_count;
        arg[ thread_num ]->total_count = &total_count;
        arg[ thread_num ]->mutex = &lock;
    }
    
    // Barrier initialization
    if( pthread_barrier_init( &barrier, NULL, NUM_THREADS ) ){
        printf( "Could not create a barrier\n" );
        return -1;
    }
    
    // Create thread1
    if( pthread_create( &worker_thread1, NULL,
                    do_work, ( void * )arg[ 0 ] ) ){
        fprintf( stderr,"Error while creating thread\n");
        exit(1);
    }
    // Create thread2
    if( pthread_create( &worker_thread2, NULL,
                    do_work, ( void * )arg[ 1 ] ) ){
        fprintf( stderr,"Error while creating thread\n");
        exit(1);
    }
    // Create thread3
    if( pthread_create( &worker_thread3, NULL,
                    do_work, ( void * )arg[ 2 ] ) ){
        fprintf( stderr,"Error while creating thread\n");
        exit(1);
    }
    
    // Join with thread
    if( pthread_join( worker_thread1, NULL ) ){
        fprintf( stderr,"Error while joining with child thread ( worker_thread1 )\n");
        exit(1);
    }
    if( pthread_join( worker_thread2, NULL ) ){
        fprintf( stderr,"Error while joining with child thread ( worker_thread2 )\n");
        exit(1);
    }
    if( pthread_join( worker_thread3, NULL ) ){
        fprintf( stderr,"Error while joining with child thread ( worker_thread3 )\n");
        exit(1);
    }
    pthread_mutex_destroy( &lock );

    fprintf( stdout, "Total sequences generated: %d\n", total_count );
    fprintf( stdout, "Number of correct sequences: %d\n", correct_count );
    
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
    int value = argument->val;
    int *correct_count = argument->correct_count;
    int *total_count = argument->total_count;
    pthread_mutex_t *mutex = argument->mutex;
    
    // loop until we have 10 correct buffer sequences
    while( *correct_count != 10 ){
        usleep( 500000 );
        // lock critical section
        pthread_mutex_lock( mutex );
        // add item to buffer
        working_index ++;
        buffer[ working_index - 1 ] = value;
        fprintf( stdout, "My id: %d\n", value );
        // unlock critical section
        pthread_mutex_unlock( mutex );
        // if the buffer is full
        if( buffer_is_full() )
            checked = false;
        
        // make threads wait until each one is done
        pthread_barrier_wait( &barrier );
        
        // lock the next critical section
        pthread_mutex_lock( mutex );
        // if the values have already been checked by a thread
        if( checked ){
            // release the lock before continuing to the top of the loop
            pthread_mutex_unlock( mutex );
            continue;
        }
        // first thread in the critical section will set checked to true
        checked = true;
        
        // reset the working index of the buffer (as a means to start
        //      overwriting the buffer )
        working_index = 0;

        if( buffer_is_correct() ){
            *correct_count += 1;
            fprintf( stdout, "123\n" );
        }
        *total_count += 1;
        // release the lock
        pthread_mutex_unlock( mutex );
    }
    // release the lock after the loop ends
    pthread_mutex_unlock( mutex );
    return NULL;
}

/* This function simply checks to see if our working index has reached
    the "NUM_THREADS" of the buffer.
*/
bool buffer_is_full(){
    if( working_index == NUM_THREADS ){
        return true;
    }
    return false;
}

/* This function accesses the global variable buffer and returns true
    if the buffer is [1,2,3] and false otherwise. */
bool buffer_is_correct(){
    int correct_buffer[ NUM_THREADS ] = { 1, 2, 3 };
    int index = 0;
    for( index = 0; index < NUM_THREADS; index++ ){
        if( buffer[ index ] != correct_buffer[ index ] )
        {
            return false;
        }
    }
    return true;
}