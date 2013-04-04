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

#include <shell.h>

#include <string.h>


/*
** PRIVATE DEFINITIONS
*/

#define INPUT_SIZE 64

/*
** PRIVATE DATA TYPES
*/

typedef struct shell_cmd {
	char *name,
	void *func
};

/*
** PRIVATE GLOBAL VARIABLES
*/

struct shell_cmd command_tab[] {
	
};

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

void interpret_input( char *input, int inputsize ) {
	
	if ( strcmp( input, "clear" ) == 0 ) {
		c_setscroll( 0, 7, 99, 99 );
		c_puts_at( 0, 6, "================================================================================" );
	}
	else if ( strcmp( input, "help" ) == 0 ) {
		
	}
	else { // Unrecognized command
		c_printf("'%s' is not recognized as an internal or external command,\n", input);
		c_printf("operable program or batch file.\n\n");
	}
	
}

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
		interpret_input( buffer, read );
	}
}

