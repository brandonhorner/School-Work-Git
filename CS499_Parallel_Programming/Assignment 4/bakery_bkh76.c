/*
Brandon Kealii Horner
Written: 10/15/2019
Cleaned up: 10/21/2019
Assignment 4:
* I used the resources and lectures provided on BBLearn for most of my
*   boilerplate thread code. This assignment specifically required the conditional
*   variable lecture. This file can be run with the check_bakery.py file provided,
*   like so:
*       (in the terminal window in the current directory)
*       gcc bakery.c -lpthreads -o bakery
*       ./bakery 3 4 5 42 | python check_bakery.py
*
* This function takes in four integer command-line arguments:
*   [# of left-handed bakers], [# of right-handed bakers], [# of cautious bakers], [seed]
*   The seed is for the random number generator. This is so that we control "randomness"
*   and can reproduce runs.
*/
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct arguments{
    int type;  // depending on this type, the baker will need to do different actions
    int number; // assign a number to the each type of baker for futher identification
    int baked_items_count; // each baker must have 10 baked items by the end of the program
    int *left_mitts_count; // each rack of mitts needs a counter
    int *right_mitts_count;
    
    pthread_mutex_t *mutex; // shared mutual exclusion lock to avoid race conditions
    // conditional variables for each kind of resource
    pthread_cond_t *left_mitts_count_not_zero;
    pthread_cond_t *right_mitts_count_not_zero;
};

void random_sleep( int a, int b );
void work( int type, int number );
void get_oven_mitts( void *arg );
void handle_cookies( int type, int number );
void put_oven_mitts_back( void *arg ); 
bool baker_is_not_done( int num_baked_items, pthread_mutex_t *mutex );
void *do_work( void *arg );

///////////////////////////////////////////////////////////////////////

#define NUM_ITERATIONS 10
#define NUM_LEFT_OVEN_MITTS 3
#define NUM_RIGHT_OVEN_MITTS 3
#define LEFT_HANDED 0
#define RIGHT_HANDED 1
#define CAUTIOUS 2

/* Helper function to sleep a random number of microseconds
 * picked between two bounds (provided in seconds).
 */
void random_sleep( int lbound_sec, int ubound_sec ){
    int num_usec;
    num_usec = lbound_sec * 100000 + 
          ( int )( (ubound_sec - lbound_sec) * 100000 *
                  (( double )(rand()) / RAND_MAX) ); 
    usleep( num_usec );
    return;
}


/*  Print out message that the thread is working, then sleep to simulate operation. */
void work( int type, int number ){
    switch( type ){
        case LEFT_HANDED:
            fprintf( stdout, "[Left-handed baker %d] is working...\n", number );
            break;
        case RIGHT_HANDED:
            fprintf( stdout, "[Right-handed baker %d] is working...\n", number );
            break;
        case CAUTIOUS:
            fprintf( stdout, "[Cautious baker %d] is working...\n", number );
            break;
    }
    random_sleep( 20, 50 ); //TODO CHANGE THIS BACK
    return;
}


/* Check for the oven mitt(s) availability, if not we wait for the mitt. */
void get_oven_mitts( void *arg ){
    
    // pass in arguments
    struct arguments *argument;
    argument = ( struct arguments* )arg;
    int baker_type = argument->type;
    int number = argument->number;
    
    // these are pointers because they will be updated later
    int *left_mitts_count = argument->left_mitts_count;
    int *right_mitts_count = argument->right_mitts_count;
    pthread_mutex_t *mutex = argument->mutex;
    pthread_cond_t *left_mitts_count_not_zero = argument->left_mitts_count_not_zero;
    pthread_cond_t *right_mitts_count_not_zero = argument->right_mitts_count_not_zero;
    
    // depending on which type of baker is grabbing a mitt, 
    //                         different options of mitts are expressed
    switch( baker_type ){
        case LEFT_HANDED:
            fprintf( stdout, "[Left-handed baker %d] wants a left-handed mitt...\n", number );
            
            // try for left-handed mitt
            while( *left_mitts_count == 0 ){
                pthread_cond_wait( left_mitts_count_not_zero, mutex );
            }
            
            fprintf( stdout, "[Left-handed baker %d] has got a left-handed mitt...\n", number );
            *left_mitts_count = *left_mitts_count - 1; // reduce the count of the mitts
            break;
            
        case RIGHT_HANDED:
            fprintf( stdout, "[Right-handed baker %d] wants a right-handed mitt...\n", number );
            
            // try for right-handed mitt
            while( *right_mitts_count == 0 ){
                pthread_cond_wait( right_mitts_count_not_zero, mutex );
            }
            
            fprintf( stdout, "[Right-handed baker %d] has got a right-handed mitt...\n", number );
            *right_mitts_count = *right_mitts_count - 1;
            break;
            
        case CAUTIOUS: // cautious bakers first take a left mitt then a right mitt
            fprintf( stdout, "[Cautious baker %d] wants a left-handed mitt...\n", number );
            
            // try for left-handed mitt
            while( *left_mitts_count == 0 ){
                pthread_cond_wait( left_mitts_count_not_zero, mutex );
            }
            
            fprintf( stdout, "[Cautious baker %d] has got a left-handed mitt...\n", number );
            *left_mitts_count = *left_mitts_count - 1;
            
            fprintf( stdout, "[Cautious baker %d] wants a right-handed mitt...\n", number );
            
            // then try for right-handed mitt
            while( *right_mitts_count == 0 ){
                pthread_cond_wait( right_mitts_count_not_zero, mutex );
            }
            
            fprintf( stdout, "[Cautious baker %d] has got a right-handed mitt...\n", number );
            *right_mitts_count = *right_mitts_count - 1;
            break;
    }
    return;
}


/* Print message saying the cookies were put in the oven. The thread will wait
    for some time, then print a message saying that the cookies were removed from
    the oven. */
void handle_cookies( int type, int number ){
    // first "put the cookies in the oven"
    switch( type ){
        case LEFT_HANDED:
            fprintf( stdout, "[Left-handed baker %d] has put cookies in the oven and is waiting...\n", number );
            break;
        case RIGHT_HANDED:
            fprintf( stdout, "[Right-handed baker %d] has put cookies in the oven and is waiting...\n", number );
            break;
        case CAUTIOUS:
            fprintf( stdout, "[Cautious baker %d] has put cookies in the oven and is waiting...\n", number );
            break;
    }
    // wait for the cookies to bake
    random_sleep( 20, 50 );
    
    // "remove the cookies"
    switch( type ){
        case LEFT_HANDED:
            fprintf( stdout, "[Left-handed baker %d] has taken cookies out of the oven...\n", number );
            break;
        case RIGHT_HANDED:
            fprintf( stdout, "[Right-handed baker %d] has taken cookies out of the oven...\n", number );
            break;
        case CAUTIOUS:
            fprintf( stdout, "[Cautious baker %d] has taken cookies out of the oven...\n", number );
            break;
    }
    return;
}


/* Print a message that the mitt(s) were put back and signal that we have put the
    mitt(s) back. */
void put_oven_mitts_back( void *arg ){
    // pass in arguments
    struct arguments *argument;
    argument = ( struct arguments* )arg;
    int baker_type = argument->type;
    int number = argument->number;
    
    // these are pointers because we will be updating them later
    int *left_mitts_count = argument->left_mitts_count;
    int *right_mitts_count = argument->right_mitts_count;
    pthread_mutex_t *mutex = argument->mutex;
    pthread_cond_t *left_mitts_count_not_zero = argument->left_mitts_count_not_zero;
    pthread_cond_t *right_mitts_count_not_zero = argument->right_mitts_count_not_zero;
    
    switch( baker_type ){
        case LEFT_HANDED:
            fprintf( stdout, "[Left-handed baker %d] has put back a left-handed mitt...\n", number );
            *left_mitts_count = *left_mitts_count + 1;
            pthread_cond_signal( left_mitts_count_not_zero );
            pthread_mutex_unlock( mutex );
            break;
            
        case RIGHT_HANDED:
            fprintf( stdout, "[Right-handed baker %d] has put back a right-handed mitt...\n", number );
            *right_mitts_count = *right_mitts_count + 1;
            pthread_cond_signal( right_mitts_count_not_zero );
            pthread_mutex_unlock( mutex );
            break;
            
        case CAUTIOUS:
            fprintf( stdout, "[Cautious baker %d] has put back a left-handed mitt...\n", number );
            *left_mitts_count = *left_mitts_count + 1;
            pthread_cond_signal( left_mitts_count_not_zero );
            
            fprintf( stdout, "[Cautious baker %d] has put back a right-handed mitt...\n", number );
            *right_mitts_count = *right_mitts_count + 1;
            pthread_cond_signal( right_mitts_count_not_zero );
            pthread_mutex_unlock( mutex );
            break;
    }
    return;
}


// checks if the baked items of the thread is 10 or not
bool baker_is_not_done( int num_baked_items, pthread_mutex_t *mutex ){
    pthread_mutex_lock( mutex );
    int ret = true;
    if( num_baked_items == 10 ){
        ret = false;
    }
    pthread_mutex_unlock( mutex );
    return ret;
}


/* Each thread will run this function to complete the job of baking 10 items.
    Bakers (threads) will have to trade off with a limited set of baking mitts
    (resources) to achieve their goal. */
void *do_work( void *arg ){
    
    // pass in arguments
    struct arguments *argument;
    argument = ( struct arguments* )arg;
    int baker_type = argument->type;
    int number = argument->number;
    int baked_items_count = argument->baked_items_count;
    int *left_mitts_count = argument->left_mitts_count;
    int *right_mitts_count = argument->right_mitts_count;
    pthread_mutex_t *mutex = argument->mutex;
    pthread_cond_t *left_mitts_count_not_zero = argument->left_mitts_count_not_zero;
    pthread_cond_t *right_mitts_count_not_zero = argument->right_mitts_count_not_zero;
    
    while( baker_is_not_done ( baked_items_count, mutex ) ){
        work( baker_type, number );

        // lock the critical section
        pthread_mutex_lock( mutex );
        get_oven_mitts( (void *) argument );
        // unlock the critical section
        pthread_mutex_unlock( mutex );

        handle_cookies( baker_type, number );

        // lock the critical section
        pthread_mutex_lock( mutex );
        put_oven_mitts_back( (void *) argument );
        baked_items_count ++;
        // unlock the critical section
        pthread_mutex_unlock( mutex );
    }
    
    return NULL;
}


int main(int argc, char **argv){

    int num_left_handed_bakers;
    int num_right_handed_bakers;
    int num_cautious_bakers;
    int seed;

    int thread_num;
    int total_single_handed_baker_threads;
    int total_baker_threads;
    int count = 0;

    // initialize a counting variable for each oven mitt (shared between baker threads)
    int l_mitt_count = NUM_LEFT_OVEN_MITTS;
    int r_mitt_count = NUM_RIGHT_OVEN_MITTS;

    // process command-line arguments 
    if( argc != 5 ){
        fprintf( stderr, "Usage: %s <# left-handed bakers> <# right-handed bakers> \
                <# cautious bakers> <seed>\n", argv[0] );
        exit(1);
    }

    if(( sscanf( argv[1], "%d", &num_left_handed_bakers ) != 1 ) || 
      ( sscanf( argv[2], "%d", &num_right_handed_bakers ) != 1 ) || 
      ( sscanf( argv[3], "%d", &num_cautious_bakers ) != 1 ) || 
      ( sscanf( argv[4], "%d", &seed ) != 1 ) ||
      ( num_left_handed_bakers < 1 ) ||
      ( num_right_handed_bakers < 1 ) ||
      ( num_cautious_bakers < 1 ) ||
      ( seed < 0 ) ){
        fprintf( stderr,"Invalid command-line arguments... Aborting\n" );
        exit(1);
    }
    // seed the random number generation
    srand( seed );

    // calculate the total amount of baker threads
    total_baker_threads = num_right_handed_bakers + num_left_handed_bakers
                                                          + num_cautious_bakers;
    // declare the baker threads as an array of threads
    pthread_t baker_thread[ total_baker_threads ];

    // declare and initialize the mutual exclusion lock
    pthread_mutex_t lock;
    pthread_mutex_init( &lock, NULL );

    // declare and initialize conditional variables for each oven mitt
    pthread_cond_t left_mitts_not_zero;
    pthread_cond_init( &left_mitts_not_zero, NULL );
    pthread_cond_t right_mitts_not_zero;
    pthread_cond_init( &right_mitts_not_zero, NULL );

    // declare an array of arguments for the threads
    struct arguments *arg[ total_baker_threads ];

    // build arguments to the 3 different baker threads
    // starting with the left-handed bakers because they are first in the main arguments
    for( thread_num = 0; thread_num < num_left_handed_bakers; thread_num ++ ){
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->baked_items_count = 0;
        arg[ thread_num ]->type = LEFT_HANDED;
        arg[ thread_num ]->number = thread_num;
        arg[ thread_num ]->left_mitts_count = &l_mitt_count;
        arg[ thread_num ]->right_mitts_count = &r_mitt_count;
        arg[ thread_num ]->mutex = &lock;
        arg[ thread_num ]->left_mitts_count_not_zero = &left_mitts_not_zero;
        arg[ thread_num ]->right_mitts_count_not_zero = &right_mitts_not_zero;
    }

    total_single_handed_baker_threads = num_left_handed_bakers + num_right_handed_bakers;

    // then build arguments to the right-handed bakers
    for( thread_num = num_left_handed_bakers; thread_num < total_single_handed_baker_threads;
                                                                                thread_num ++ ){
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->baked_items_count = 0;
        arg[ thread_num ]->type = RIGHT_HANDED;
        arg[ thread_num ]->number = count ++;
        arg[ thread_num ]->left_mitts_count = &l_mitt_count;
        arg[ thread_num ]->right_mitts_count = &r_mitt_count;
        arg[ thread_num ]->mutex = &lock;
        arg[ thread_num ]->left_mitts_count_not_zero = &left_mitts_not_zero;
        arg[ thread_num ]->right_mitts_count_not_zero = &right_mitts_not_zero;
    }
    // reset the count for assigning numbers to bakers
    count = 0;

    // finally build agruments to the cautious bakers
    for( thread_num = total_single_handed_baker_threads;
            thread_num < total_baker_threads; thread_num ++ ){
        arg[ thread_num ] = ( struct arguments * )calloc( 1, sizeof ( struct arguments ) );
        arg[ thread_num ]->baked_items_count = 0;
        arg[ thread_num ]->type = CAUTIOUS;
        arg[ thread_num ]->number = count ++;
        arg[ thread_num ]->left_mitts_count = &l_mitt_count;
        arg[ thread_num ]->right_mitts_count = &r_mitt_count;
        arg[ thread_num ]->mutex = &lock;
        arg[ thread_num ]->left_mitts_count_not_zero = &left_mitts_not_zero;
        arg[ thread_num ]->right_mitts_count_not_zero = &right_mitts_not_zero;
    }

    // create total_baker_threads worth of threads, each calling do_work()
    for( thread_num = 0; thread_num < total_baker_threads; thread_num++ ){
        if( pthread_create( &baker_thread[ thread_num ], NULL,
                        do_work, ( void * )arg[ thread_num ] ) ){
            fprintf ( stderr,"Error while creating thread %d\n", thread_num );
            exit(1);
        }
    }

    // join all of the baker threads
    for( thread_num = 0; thread_num < total_baker_threads; thread_num++ ){
        if( pthread_join( baker_thread[ thread_num ], NULL ) ){
            fprintf ( stderr,"Error while joining with child thread %d\n", thread_num );
            exit(1);
        }    
    }

    pthread_mutex_destroy( &lock );

    exit(0);
}

