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

#define INPUT_LEN 64
#define MAX_ARGC 10

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

char *ps1 = "C:\\Users\\team1>";

int command_count = 4;
shell_cmd command_tab[] = {
	{"ps", run_ps},
	{"clear", run_clear},
	{"help", run_help},
	{"reboot", run_reboot}
};

char buffer[INPUT_LEN];
char *argv[MAX_ARGC];

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/


void interpret_input( int argc, char **argv ) {
	
	pid_t pid;
	pid_t upid;
	ustatus_t status;
	estatus_t estat;
	
	int i;
	for ( i = 0; i != command_count; ++i ) {
		if ( strcmp( command_tab[i].name, argv[0] ) == 0 ) {
			pid = fork( PRIO_STD );
			
			if ( pid == -1 ) { // Error

				c_printf("Fork returned error, %s not completed.\n", argv[0]);
				
			} else if ( pid == 0 ) { // Child

				status = execv( command_tab[i].func, argc, argv);
				
				// Should not return
				c_printf("Warning: execv returned with status %d\n", status);
				exit( EXIT_FAILURE );
			} else { // Parent

				status = wait( &upid, &estat );
				
			}
			return;
		}
	}
	
	// Unrecognized command
	c_printf("'%s' is not recognized as an internal or external command,\n", argv[0]);
	c_printf("operable program or batch file.\n\n");
	
}

/*
** PUBLIC FUNCTIONS
*/

void run_shell( void ) {
	int read;
	
	while ( 1 ) {
		// wait on kb interrupt
		c_printf(ps1);
		read = c_gets( buffer, INPUT_LEN );
		buffer[read-1] = 0;
		
		if ( read != 1 ) {
			int i = 0;
			int argc = 0;
			
			do {
				argv[argc] = &buffer[i];
				++argc;
				
				// Find the first space
				for (;buffer[i] != ' ' && i < read; ++i);
				
				// Remove excess whitespace
				for (;buffer[i] == ' '; ++i) {
					buffer[i] = 0;
				}
				
			} while ( i != read );
			
			interpret_input( argc, argv );
		}
	}
}

void wall_s (char *message)
{
	c_printf("%s", message);
	for (int i = 0; i < strlen(message); ++i)
	{
		write(message[i]);
	}
}

void wall_ch (char ch)
{
	c_putchar(ch);
	write(ch);
}
