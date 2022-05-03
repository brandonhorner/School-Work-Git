/*
Brandon Kealii Horner
9/26/2019 
Assignment 3: Question 2
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate thread code. I also re-used much of question 1 since this question
  is very similar. This file can be run with the A3sequence2_test.py file
  provided, like so:
        (in the terminal window in the current directory)
        gcc .q2.c -lpthreads -o .q2
        ./q2 | python A3sequence2_test.py
*/
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 3
#define TOTAL_THREADS 6

struct arguments {
    int val;
    int *correct_count;
    int *total_count;
    pthread_mutex_t *mutex;
};

void *do_work(void *arg);
void *do_work2(void *arg);
bool buffer_is_correct();
bool buffer2_is_correct();
bool buffer_is_full();
bool buffer2_is_full();

// globally shared buffer
int buffer[ NUM_THREADS ];
int buffer2[ NUM_THREADS ];

// create index for storing thread values
int team1_threadid_index = 0;
int team2_threadid_index = 0;

// index of the buffer to be added to
int working_index2 = 0;
int working_index = 0;

// a flag to see if a thread has already checked the buffer contents
bool checked = false;
bool checked2 = false;
bool flag = true;

pthread_barrier_t barrier;

/* This code differs from the previous question because two "teams" of
    three threads are "racing" to complete 10 correct buffer sequences.
    Either "1, 2, 3" or "4, 5, 6" is a correct buffer for team 1 and
    team 2 respectively. Once one thread "wins" it signals the other
    threads to stop executing.
*/
int main(int argc, char *argv){
    
    int correct_count = 0;
    int total_count = 0;
    int correct_count2 = 0;
    int total_count2 = 0;
    int thread_num;
    
    pthread_t worker_thread[ TOTAL_THREADS ];
    
    pthread_mutex_t lock;
    pthread_mutex_init( &lock, NULL );
    
    pthread_mutex_t lock2;
    pthread_mutex_init( &lock2, NULL );
   
    struct arguments *arg[ TOTAL_THREADS ];
    
    // Build arguments to first team of threads
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ ){
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->val = thread_num + 1;
        arg[ thread_num ]->correct_count = &correct_count;
        arg[ thread_num ]->total_count = &total_count;
        arg[ thread_num ]->mutex = &lock;
    }
    // Build arguments to second team of threads
    for( thread_num = 3; thread_num < TOTAL_THREADS; thread_num++ ){
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->val = thread_num + 1;
        arg[ thread_num ]->correct_count = &correct_count2;
        arg[ thread_num ]->total_count = &total_count2;
        arg[ thread_num ]->mutex = &lock2;
    }
    
    // Barrier initialization
    if( pthread_barrier_init( &barrier, NULL, NUM_THREADS ) ){
        printf( "Could not create a barrier\n" );
        return -1;
    }
    
    // Create first team of three threads calling do_work()
    for( thread_num = 0; thread_num < NUM_THREADS; thread_num++ ){

        if( pthread_create( &worker_thread[ thread_num ], NULL,
                        do_work, ( void * )arg[ thread_num ] ) ){
            fprintf ( stderr,"Error while creating thread %d\n", thread_num );
            exit(1);
        }
    }
    // Create second team of three threads calling do_work2()
    for( thread_num = 3; thread_num < TOTAL_THREADS; thread_num++ ){
        
        if( pthread_create( &worker_thread[ thread_num ], NULL,
                        do_work2, ( void * )arg[ thread_num ] ) ){
            fprintf ( stderr,"Error while creating thread\n");
            exit(1);
        }
    }
    // Join all 6 threads with main thread
    for( thread_num = 0; thread_num < TOTAL_THREADS; thread_num++ ){
        if( pthread_join( worker_thread[ thread_num ], NULL ) ){
            fprintf ( stderr,"Error while joining with child thread %d\n", thread_num );
            exit(1);
        }    
    }
    
    pthread_mutex_destroy( &lock );
    pthread_mutex_destroy( &lock2 );
    
    if( correct_count > correct_count2 )
        fprintf( stdout, "Team 1 won!\n" );
    else
        fprintf( stdout, "Team 2 won!\n" );
    
    fprintf( stdout, "Total sequences generated team1: %d\n", total_count );
    fprintf( stdout, "Number of correct sequences team1: %d\n", correct_count );
    fprintf( stdout, "Total sequences generated team2: %d\n", total_count2 );
    fprintf( stdout, "Number of correct sequences team2: %d\n", correct_count2 );    
    
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
    while( flag ){
        usleep( 500 );
        
        // lock critical section
        pthread_mutex_lock( mutex );
        
        // first check if the loop should be false or not
        if( *correct_count == 10 ){
            flag = false;
            pthread_mutex_unlock( mutex );
            continue;
        }
        // add item to buffer
        working_index ++;
        buffer[ working_index - 1 ] = value;
        fprintf( stdout, "My id: %d\n", value );
        
        // if the buffer is full set the flag to false (to avoid risk 
        //  of changing checked before the other threads finish)
        if( buffer_is_full() )
            checked = false;
        
        // unlock critical section
        pthread_mutex_unlock( mutex );
        
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
    // release the lock after the loop ends (for remaining threads)
    pthread_mutex_unlock( mutex );
    return NULL;

}

void *do_work2( void *arg ){
    // Pass in arguments
    struct arguments *argument;
    argument = ( struct arguments* )arg;  
    int value = argument->val;
    int *correct_count = argument->correct_count;
    int *total_count = argument->total_count;
    pthread_mutex_t *mutex = argument->mutex;
    // loop until we have 10 correct buffer sequences
    while( flag ){
        usleep( 500 );
        
        // lock critical section
        pthread_mutex_lock( mutex );
        // first check if the loop should be false or not
        if( *correct_count == 10 ){
            flag = false;
            pthread_mutex_unlock( mutex );
            continue;
        }
        // add item to buffer
        working_index2 ++;
        buffer2[ working_index2 - 1 ] = value;
        fprintf( stdout, "My id: %d\n", value );
        
        // if the buffer is full set the flag to false (to avoid risk 
        //  of changing checked before the other threads finish)
        if( buffer2_is_full() )
            checked2 = false;
        
        // unlock critical section
        pthread_mutex_unlock( mutex );
        
        // make threads wait until each one is done
        pthread_barrier_wait( &barrier );
        
        // lock the next critical section
        pthread_mutex_lock( mutex );
        // if the values have already been checked by a thread
        if( checked2 ){
            // release the lock and continue to the top of the loop
            pthread_mutex_unlock( mutex );
            continue;
        }
        // first thread in the critical section will set checked to true
        checked2 = true;
        
        // reset the working index of the buffer ( as a means to start
        //      overwriting the buffer )
        working_index2 = 0;

        if( buffer2_is_correct() ){
            *correct_count += 1;
            fprintf( stdout, "456\n" );
        }
        *total_count += 1;
        // release the lock
        pthread_mutex_unlock( mutex );
    }
    // release the lock after the loop ends (for remaining threads)
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

bool buffer2_is_full(){
    if( working_index2 == NUM_THREADS ){
        return true;
    }
    //printf("buffer2_is_full() was false:\n  working_index2 = %d\n,   NUM_THREADS = %d\n", working_index2, NUM_THREADS);
    return false;
}

/* This function accesses the global variable buffer and returns true
    if the buffer is [ 1, 2, 3 ] and false otherwise. */
bool buffer_is_correct(){
    int correct_buffer[ NUM_THREADS ] = { 1, 2, 3 };
    int index;
    for( index = 0; index < NUM_THREADS; index++ ){
        if( buffer[ index ] != correct_buffer[ index ] ){
            return false;
        }
    }
    return true;
}

bool buffer2_is_correct(){
    int correct_buffer2[ NUM_THREADS ] = { 4, 5, 6 };
    int index;
    for( index = 0; index < NUM_THREADS; index++ ){
        if( buffer2[ index ] != correct_buffer2[ index ] ){
            //printf( "buffer2[index] = %d correct_buffer2[index] = %d\n", buffer2[index], correct_buffer2[index]);
            return false;
        }
    }
    return true;
}

