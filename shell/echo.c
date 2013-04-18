/*
** File: shell/echo.c
**
** Author: Owen Royall-Kahin
**
** Description: echo command
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

void run_echo( int argc, char **argv ) {
	int i;
	for ( i = 1; i != argc; ++i ) {
		c_printf( "%s ", argv[i] );
	}
	c_printf( "\n" );
}
