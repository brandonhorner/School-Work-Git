/*
Brandon Kealii Horner
9/27/2019
Assignment 3: Question 4
    I used the resources and lectures provided on BBLearn for most of my
  boilerplate thread code. This file can be run with the A3pingpong_test.py
  file provided, like so:
        (in the terminal window in the current directory)
        gcc .q4.c -lpthreads -o .q4
        ./q4 | python A3pingpong_test.py
*/
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 2

void *increment_count();
void *decrement_count();

// global variables
pthread_mutex_t mutex;
pthread_cond_t count_not_zero;
pthread_cond_t count_not_ten;

unsigned int count = 0;
int total_count = 0;

/* This program uses 2 threads. One thread increments a counter to 10, then
    the other decrements the same counter back to 0, repeating the incrementing
    and decrementing until we have incremented 30 times and decremented 20 times.
    When one of the threads is changing the count, the other one waits.
*/
int main(int argc, char *argv){
    // declare both threads
    pthread_t increment_thread;
    pthread_t decrement_thread;

    // initialize the mutual exclusion lock
    pthread_mutex_init( &mutex, NULL );
    
    // create both threads
    if( pthread_create( &increment_thread, NULL,
                       increment_count, NULL ) ){
        fprintf( stderr, "Error while creating thread\n" );
        exit(1);
    }
    if( pthread_create( &decrement_thread, NULL,
                       decrement_count, NULL ) ){
        fprintf( stderr, "Error while creating thread\n" );
        exit(1);
    }
    
    // join both threads
    if( pthread_join( increment_thread, NULL ) ){
        fprintf( stderr, "Error while trying to join increment_thread\n" );
        exit(1);
    }
    if( pthread_join( decrement_thread, NULL ) ){
        fprintf( stderr, "Error while trying to join decrement_thread\n" );
        exit(1);
    }
    
    exit(0);
}

void *decrement_count(){
    while( true ){
        // lock critical section
        pthread_mutex_lock( &mutex );
        
        while( count == 0 ){
            // wait for count to not be 0
            pthread_cond_wait( &count_not_zero, &mutex );
            // decrement count finished waiting  
        }
        
        // if we reached 50 iterations, we should not decrement again
        if( total_count == 50 ){
            pthread_cond_signal( &count_not_ten );
            pthread_mutex_unlock( &mutex );
            return NULL;
        }
        
        // decrement the count 10 times
        while( count != 0 ){
            count -= 1;
            total_count++;
            fprintf( stdout, "Count is now (dec fn): %d\n", count );
            usleep( 500000 );
        }
        // signal any threads waiting for the count to not be 10
        pthread_cond_signal( &count_not_ten );
        pthread_mutex_unlock( &mutex );
    }
    return NULL;
}

void *increment_count(){
    while( true ){
        // lock critical section
        pthread_mutex_lock( &mutex );
        
        // if we reached 50 iterations, we should not begin incrementing again
        if( total_count == 50 ){
            // unlock the critical section before returning
            pthread_mutex_unlock( &mutex );
            return NULL;
        }
        
        while( count == 10 ){
            // wait for count to not be 10
            pthread_cond_wait( &count_not_ten, &mutex );
            // increment count finished waiting
        }
        
        // increment the count 10 times
        while( count != 10 ){
            count++;
            total_count++;
            fprintf( stdout, "Count is now (inc fn): %d\n", count );
            usleep( 500000 );
        }
        // signal any threads waiting for the count to not be 0
        pthread_cond_signal( &count_not_zero );
        // unlock the critical section
        pthread_mutex_unlock( &mutex );
    }
    return NULL;
}











