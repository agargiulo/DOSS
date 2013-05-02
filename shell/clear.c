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
		uint8_t y;
#ifdef ISR_DEBUGGING_CODE
		y = 7;
#else
		y = 0;
#endif
		for (; y < 99; ++y)
		{
			c_putchar_at(x, y, ' ');
		}
	}
	c_moveto(0,0);
}
