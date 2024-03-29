/*
** File: shell/halt.c
**
** Author: Anthony Gargiulo
**
** Description: halts the computer
*/
#include <common.h>
#include <string.h>
#include <shell.h>

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

// WILL NOT RETURN
void run_halt(int argc, char **argv) {
	char *halt_message = "System going down for halt in ";
	int countdown;

	if (argc >= 2)
	{
		countdown = atoi(argv[1]);
		if (countdown < 0 || countdown > 9)
		{
			c_puts("Try again with a number between 0 and 9 inclusive\n");
			return;
		}
	}
	else
	{
		countdown = 1;
	}

	wall_s(halt_message);

	write('\a');
	for (; countdown > 0; --countdown)
	{
		wall_ch( (char) countdown + 48);
		sleep(20);
		wall_ch('.');
		sleep(20);
		wall_ch('.');
		sleep(20);
	}
	wall_s("NOW");
	sleep(20);
	write('\a');
	__asm("cli");
	for(;;) ;
}
