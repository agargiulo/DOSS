/*
** File: shell/nettest.c
**
** Author: Anthony Gargiulo
**
** Description: test network things from the shell
*/

#include <common.h>
#include <startup.h>
#include <8255x.h>

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
void run_nettest(int argc, char **argv)
{
	net_pci_dump();
	c_puts("---MORE---");
	c_getchar();
	c_puts("\n");
	net_CSR_dump();
}
