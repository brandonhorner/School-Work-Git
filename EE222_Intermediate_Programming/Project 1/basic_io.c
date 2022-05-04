#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BUFFER_SIZE 16
/*Brandon Horner Project 1
**/

float get_launch_height();
float get_launch_angle();
float get_launch_velocity();
void get_input(char*);
float parse_float(char*, float);

int main(void) 
{
  float g = 9.8;
  float launch_height = get_launch_height();
  float launch_angle = get_launch_angle();
  float launch_velocity = get_launch_velocity();
  float horizontal_velocity = abs((cosf(launch_angle) *  launch_velocity));
  float vertical_velocity = abs((sinf(launch_angle) * launch_velocity));
  float time = (2 * (vertical_velocity/g)) + ((launch_height*16)/(vertical_velocity*15));
  float horizontal_distance = horizontal_velocity * time;
  printf("Hello! Please enter the information as prompted: \n");
  printf("launch height: %f m \n", launch_height);
  printf("launch angle: %f degrees \n", launch_angle);
  printf("launch velocity: %f m/s \n", launch_velocity);
	
  if(launch_angle>0)
  {
	printf("horizontal velocity: %f m/s \n", horizontal_velocity);
    printf("vertical velocity: %f m/s \n", vertical_velocity);
	printf("horizontal distance: %f m \n", horizontal_distance);
  }
  printf("time to impact: %f s \n\n", time);
  return 0;
}

float get_launch_height() 
{
  char input_buffer[BUFFER_SIZE];
  float launch_height = -1;

  /*Until the user gives us usable input (number >= 0) 
   * do the following...
   **/
  while( launch_height < 0 ) 
  {
    printf( "Please enter a launch height (in meters):\n\t" );
    get_input( input_buffer );
    
    launch_height = parse_float( input_buffer, -1 );
    
    if( launch_height < 0 ) 
    {
       printf("Error: Please enter a height >= 0!\n");
    }
  } 
  return launch_height;
}

float get_launch_angle()
{
    char input_buffer[BUFFER_SIZE];
    float launch_angle = -91;
    
    while(launch_angle < -90 || launch_angle > 90)
    {
       printf( "Please enter a launch angle (in degrees):\n\t" );
       get_input( input_buffer );

       launch_angle = parse_float( input_buffer, - 1 ); 

       if( launch_angle < -90 || launch_angle > 90 ) 
       {
           printf("Input Error: Expected an angle between 90 and -90\n" );
       }
    }
    return launch_angle;
}

float get_launch_velocity()
{
	char input_buffer[BUFFER_SIZE];
    float launch_velocity = -1;

    while( launch_velocity < 0 )
    {
       printf( "Please enter a launch velocity in meters per second:\n\t" );
       get_input( input_buffer );

       launch_velocity = parse_float( input_buffer, - 1 ); 

       if( launch_velocity < 0 ) 
       {
           printf("Input Error: Expected a velocity > 0!\n" );
       }
    }
    return launch_velocity;
}


 /*Read in the user's input, and quit if there's an issue
  **/
void get_input( char *input_buffer ) 
{
  if( fgets ( input_buffer, BUFFER_SIZE, stdin ) == NULL ) 
  {
    printf("Failed to get correct input!\n");
    exit( EXIT_FAILURE );
  }
}

/* This function attempts to parse a float from input_buffer.
 * On success it returns the parsed float, and on error it
 * returns default_return.
 **/
float parse_float(char *input_buffer, float default_return) 
{
    char *end_ptr;
    float parsed_float = strtof(input_buffer, &end_ptr);
    /* If end_ptr is poiting to the first char of
     * input_buffer, there was a problem.
     **/
    if(end_ptr == input_buffer) 
    {
      return default_return;
    }
    return parsed_float;
}


