/*
** SCCS ID:	%W%	%G%
**
** File:	shell.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Shell implementation for OS2
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <shell.h>
#include <c_io.h>

#define INPUT_SIZE 64

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

void run_shell( void ) {
	int read;
	char buffer[INPUT_SIZE];
	
	while ( 1 ) {
		// wait on kb interrupt
		c_printf("C:\\Users\\team1>");
		read = c_gets( buffer, INPUT_SIZE );
		buffer[read-1] = 0;
		c_printf("'%s' is not recognized as an internal or external command,\n", buffer);
		c_printf("operable program or batch file.\n\n");
	}
}

