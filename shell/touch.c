/*
**
** File:	touch.c
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	Implementation of touch for ofs
*/

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

void run_touch(int argc, char **argv) {
	if ( argc > 1 ) {
		fcreat( argv[1] );
	}
}