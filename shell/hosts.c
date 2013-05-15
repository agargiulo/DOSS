/*
** File: shell/hosts.c
**
** Author: Anthony Gargiulo
**
** Description: list various information about hosts on the local area network
*/

#include <common.h>
#include <string.h>
#include <net.h>

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

void run_hosts(int argc, char *argv[])
{
	if (argc < 2)
	{
		c_puts("Usage: hosts [-h|-a|-l|<hostname>]\n");
		return;
	}
	if (strcmp(argv[1], "-h") == 0)
	{
		c_puts("Usage: hosts [-h|-a|-l|hostname]\n");
		c_puts("\nThis command shows various bits of information about network related things\n\n");
		c_puts("-h : prints this message\n");
		c_puts("-a : prints information about every known host\n");
		c_puts("-l : prints information about this machine\n");
		c_puts("<hostname> : prints information about the given hostname\n");
		return;
	}
	else if (strcmp(argv[1], "-l") == 0)
	{
		c_printf("Hostname: %s\n", get_hostname(eth0.hw_addr));
	}
}
