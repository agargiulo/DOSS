/*
** File: shell/help.c
**
** Author: Owen Royall-Kahin
**
** Description: help command
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
	
	c_printf("Welcome to DOSS!\n");
	c_printf("Currently, the following commands are supported:\n");
	c_printf("--> help: Display this dialog\n");
	c_printf("--> clear: Clear the screen\n");
	c_printf("--> lspci: List all of the PCI devices\n");
	c_printf("--> reboot: Reboot the computer\n");
	c_printf("Much more soon to come!\n");
	
}
