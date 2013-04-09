/*
** SCCS ID:	%W%	%G%
**
** File:	?
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <string.h>

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
	//if (argc > 1
	char *reboot_message = "System going down for reboot in ";
	c_printf("%s", reboot_message);
	for (int i = 0; i < strlen(reboot_message); i++)
	{
		write(reboot_message[i]);
	}
	
	for (uint8_t countdown = 9; countdown > 0; --countdown)
	{
		c_printf("%d", countdown);
		write(countdown + 48);
		sleep(ms_to_ticks(20));
		c_printf(".");
		write('.');
		sleep(ms_to_ticks(20));
		c_printf(".");
		write('.');
		sleep(ms_to_ticks(20));
	}
	c_printf("NOW");
	write('N');
	write('O');
	write('W');
	sleep(ms_to_ticks(20));
	__asm__("lidt 0");
}
