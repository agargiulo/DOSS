/*
**
** File:	append.c
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	Function for appending data to files in ofs, for testing 
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

void run_append(int argc, char **argv) {
	if ( argc > 2 ) {
		file_t * file = fopen( argv[1] );
		if ( file != 0 ) {
			if ( file->size == 0 ) {
				fwrite( argv[2], sizeof(char), strlen(argv[2]), file );
				file->size = strlen(argv[2]);
				
			} else {
				uint32_t newsize = (file->size) + strlen(argv[2]);
				char *buffer = qalloc( newsize );
				fread( buffer, sizeof(char), file->size, file );
				memcpy( buffer + (file->size), argv[2], strlen(argv[2]) );
				fwrite( buffer, sizeof(char), newsize, file );
				file->size = newsize;
			}
			fclose( file );
		}
		
	}
}