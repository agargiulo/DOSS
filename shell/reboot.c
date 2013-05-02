/*
** File: shell/reboot.c
**
** Author: Anthony Gargiulo
**
** Description: reboots the computer using fun triple-fault magic
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
void run_reboot(int argc, char **argv) {
	char *reboot_message = "System going down for reboot in ";
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

	wall_s(reboot_message);

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
	__asm__("lidt 0");
}
