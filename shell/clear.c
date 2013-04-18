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

void run_clear( int argc, char **argv ) {
	c_setscroll( 0, 7, 99, 99 );
	c_puts_at( 0, 6, "================================================================================" );
	for (uint8_t x = 0; x < 99; ++x)
	{
		for (uint8_t y = 7; y < 99; ++y)
		{
			c_putchar_at(x, y, ' ');
		}
	}
}
