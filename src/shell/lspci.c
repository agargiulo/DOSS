/*
** File: shell/lspci.c
**
** Author: Anthony Gargiulo
**
** Description: list PCI devices in the shell
*/
#include <common.h>
#include <string.h>
#include <pci.h>

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

void run_lspci(int argc, char **argv)
{
	device_t *dev;
	for (int i = 0; i < device_count; i++)
	{
		dev = &device_tab[i];
		c_printf("%02x:%02x:%01x class: 0x%02x:%02x devID: 0x%04x vendor: 0x%04x\n",
				dev->bus, dev->slot, dev->func, dev->class, dev->subclass,
				dev->device, dev->vendor);
	}
}
