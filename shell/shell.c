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
	char *name;
	void *func;
}shell_cmd;

/*
** PRIVATE GLOBAL VARIABLES
*/
int command_count = 4;
shell_cmd command_tab[] = {
	{"ps", run_ps},
	{"clear", run_clear},
	{"help", run_help},
	{"reboot", run_reboot}
};

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

void interpret_input( char *input, int inputsize ) {
	ustatus_t status;
	pid_t pid;
	
	int i;
	for ( i = 0; i != command_count; ++i ) {
		if ( strcmp( command_tab[i].name, input ) == 0 ) {
			pid = fork( PRIO_STD );
			
			if ( pid == -1 ) {
				// error
				
			} else if ( pid == 0 ) {
				status = exec( command_tab[i].func );
				// Should not return
			}
			// else parent, return
			return;
		}
	}
	
	// Unrecognized command
	c_printf("'%s' is not recognized as an internal or external command,\n", input);
	c_printf("operable program or batch file.\n\n");
	
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

