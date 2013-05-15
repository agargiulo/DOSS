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

#define	__SP2_KERNEL__

#include <common.h>
#include <ofs.h>

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

void run_stat(int argc, char **argv) {
	if ( argc > 1 ) {
		stat( argv[1] );
	}
}