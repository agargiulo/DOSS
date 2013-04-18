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
	wall_s(reboot_message);
	
	write('\a');
	for (uint8_t countdown = 9; countdown > 0; --countdown)
	{
		wall_ch( (char) countdown + 48);
		sleep(ms_to_ticks(20));
		wall_ch('.');
		sleep(ms_to_ticks(20));
		wall_ch('.');
		sleep(ms_to_ticks(20));
	}
	wall_s("NOW");
	sleep(ms_to_ticks(20));
	write('\a');
	__asm__("lidt 0");
}
