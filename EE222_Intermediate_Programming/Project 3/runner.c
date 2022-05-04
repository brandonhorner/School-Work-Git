#include <stdlib.h>
#include <stdio.h>
#include "mazelib.h"

/*Brandon Horner EE222 Project 3*/

static bool is_at_end(void);
static char get_facing_char(void);
static void set_breadcrumb(void);
static void turn_right(void);
static void turn_left(void);
static void move_forward(void);
int x_pos;
int y_pos;
direction facing;

/*initializes position to start (1,1)*/
void runner_init(void)
{
	x_pos = 1;
	y_pos = 1;
	facing = SOUTH;
	return;
}

void runner_solve(void)
{
	char forward_path;
	char right_path;
	char left_path;
	while ( !is_at_end() )
	{
		forward_path = get_facing_char();
		turn_right();
		right_path = get_facing_char();
		turn_right();
		turn_right();
		left_path = get_facing_char();
		/*reorient*/
		turn_right();
		
		/*if wall to right is open go that way*/
		if(  right_path != '#' )
		{
			/*right hand rule, we see if right path is open*/
			turn_right();
			move_forward();
			set_breadcrumb();
		}
		else if( right_path == '#' )
		{
			/*if we can't go right, go forward*/
			if( forward_path != '#' )
			{
				move_forward();
				set_breadcrumb();
			}
			/*if we can't go right or forward*/
			else if ( forward_path == '#' && left_path != '#' )
			{
				turn_left();
				move_forward();
				set_breadcrumb();
			}
			/*if we can't go right, or forward, or left*/
			else
			{
				turn_right();
				turn_right();
				move_forward();
				set_breadcrumb();
			}
		}
	}
	return;
}

bool is_at_end(void)
{
	char current = maze_get_char( x_pos, y_pos );
	if ( current == 'E' )
	{
		return true;
	}
	return false;
}

char get_facing_char(void)
{
	char wall_check;
	if ( facing == NORTH )
	{
		wall_check = maze_get_char( x_pos, y_pos - 1 );
	}
	else if ( facing == EAST )
	{
		wall_check = maze_get_char( x_pos + 1, y_pos );
	}
	else if ( facing == SOUTH )
	{
		wall_check = maze_get_char( x_pos, y_pos + 1 );
	}
	else
	{
		wall_check = maze_get_char( x_pos - 1, y_pos );	
	}
	return wall_check;
}
void set_breadcrumb(void)
{
	if ( maze_get_char( x_pos, y_pos ) == 'E' )
	{
		return;
	}
	else if ( maze_get_char( x_pos, y_pos ) == ' ' )
	{
		maze_set_char( x_pos, y_pos, '.' );
	}
	else if( maze_get_char( x_pos, y_pos ) == '.' )
	{
		maze_set_char( x_pos, y_pos, 'o' );
	}
	
	else if( maze_get_char( x_pos, y_pos ) == 'o' )
	{
		maze_set_char( x_pos, y_pos, 'O' );
	}
	else
	{
		maze_set_char( x_pos, y_pos, '@' );	
	}
	return;
}

void turn_right(void)
{
	if( facing == NORTH )
	{
		facing = EAST;
	}
	else if ( facing == EAST )
	{
		facing = SOUTH;
	}
	else if ( facing == SOUTH )
	{
		facing = WEST;
	}
	else
	{
		facing = NORTH;
	}
	return;
}

void turn_left(void)
{
	turn_right();
	turn_right();
	turn_right();
	return;
}

void move_forward(void)
{
	if( facing == NORTH )
	{
		y_pos -- ;
	}
	else if ( facing == SOUTH )
	{
		y_pos ++;
	}
	else if ( facing == EAST )
	{
		x_pos ++;
	}
	else /*facing == WEST*/
	{
		x_pos --;
	}
	return;
}