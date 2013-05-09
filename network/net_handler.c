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
	// c_printf("\nVector=0x%02x, code=%d\n", vector, code );
	uint16_t SCB_STAT_ACK  = __inb(CSR_BAR + E_CSR_SCB_STAT_ACK);
	uint16_t SCB_STAT_Byte = __inb(CSR_BAR + E_CSR_SCB_STAT_WORD);
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_ACK, SCB_STAT_Byte);

	if ((SCB_STAT_ACK & 0x80) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0X08);
	}
	if ((SCB_STAT_ACK & 0x40) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0x40);
	}
	if ((SCB_STAT_ACK & 0x20) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0x20);
	}
	if ((SCB_STAT_ACK & 0x10) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0x10);
	}
	if ((SCB_STAT_ACK & 0x08) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0x08);
	}
	if ((SCB_STAT_ACK & 0X04) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0X04);
	}
	if ((SCB_STAT_ACK & 0X02) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0X02);
	}
	if ((SCB_STAT_ACK & 0X01) != 0)
	{
		__outb(CSR_BAR + E_CSR_SCB_STAT_ACK, 0X01);
	}

	SCB_STAT_ACK  = __inb(CSR_BAR + E_CSR_SCB_STAT_ACK);
	SCB_STAT_Byte = __inb(CSR_BAR + E_CSR_SCB_STAT_WORD);
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_ACK, SCB_STAT_Byte);
	

	 __outb( PIC_MASTER_CMD_PORT, PIC_EOI );
	if( vector >= 0x28 && vector <= 0x2f )
	{
		__outb( PIC_SLAVE_CMD_PORT, PIC_EOI );
	}

}
