/*
** SCCS ID:	%W%	%G%
**
** File:	?
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
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