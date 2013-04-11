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

void run_help( int argc, char **argv ) {
	int i;
	for ( i = 0; i != argc; ++i ) {
		c_printf( "%s\n", argv[i] );
	}
	
	c_printf( "help called\n" );
	
}
