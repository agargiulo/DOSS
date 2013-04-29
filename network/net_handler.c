/*
** File: network/net_handler.c
**
** Author: Anthony Gargiulo
**
** Description: ISR for the 8255x card
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <x86arch.h>

#include <startup.h>
#include <pci.h>
#include <8255x.h>
#include <net_handler.h>

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

void _net_handler(int vector, int code)
{
	uint8_t SCB_STAT_Byte = __inb(CSR_BAR + E_CSR_SCB_STAT_WORD + 1);
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_Byte, __inb(CSR_BAR + E_CSR_SCB_STAT_WORD));
	if ((SCB_STAT_Byte & 0x80) == 0x80)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_WORD + 1, 0x80);
	}
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_Byte, __inb(CSR_BAR + E_CSR_SCB_STAT_WORD));

	__outb( PIC_MASTER_CMD_PORT, PIC_EOI );
}
