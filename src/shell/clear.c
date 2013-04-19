/*
** File: shell/clear.c
**
** Author: Anthony Gargiulo
**
** Description: clears the screen in the shell
*/

#include <common.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

void run_clear( int argc, char **argv )
{
	for (uint8_t x = 0; x < 99; ++x)
	{
		for (uint8_t y = 0; y < 99; ++y)
		{
			c_putchar_at(x, y, ' ');
		}
	}
	c_moveto(0,0);
}
