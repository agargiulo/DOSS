/*
**
** File:	cat.c
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	Implemenation of cat shell function for ofs
*/

#include <common.h>
#include <ofs.h>
#include <qalloc.h>

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

void run_cat(int argc, char **argv) {
	if ( argc > 1 ) {
		file_t * file = fopen( argv[1] );
		if ( file != 0 ) {
			char *buffer = qalloc( file->size );
			fread( buffer, 1, file->size, file );
			fclose( file );
			c_printf("%s\n", buffer);
		}
	}
}