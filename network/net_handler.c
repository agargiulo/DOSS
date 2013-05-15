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

static int first_interrupt = 1;

void _net_handler(int vector, int code)
{
	if (first_interrupt)
	{
		first_interrupt = 0;
		__outb( PIC_MASTER_CMD_PORT, PIC_EOI );
		__outb( PIC_SLAVE_CMD_PORT, PIC_EOI );
		return;
	}

	uint16_t SCB_STAT_ACK_Byte  = __inb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK);
	uint16_t SCB_STAT_Byte = __inb(eth0.CSR_BAR + E_CSR_SCB_STAT_WORD);
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_ACK_Byte, SCB_STAT_Byte);

	if ((SCB_STAT_ACK_Byte & SCB_STAT_CX) != 0)
	{
		eth0.CU_finished = 1;
		c_printf("CU Command complete (%d)\n", eth0.CU_finished);
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_CX);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_FR) != 0)
	{
		c_puts("Frame recieved\n");

		e100_rx_buf_t *rx_buffer = eth0.rx_buf_ptr;
		while (rx_buffer->header.stat != 0)
		{
			if (rx_buffer->header.link_offset == (uint32_t) eth0.rx_buf_ptr)
			{
				break;
			}

			eth0.rx_count++;
		}

		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_FR);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_CNA) != 0)
	{
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_CNA);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_RNR) != 0)
	{
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_RNR);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_MDI) != 0)
	{
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_MDI);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_SWI) != 0)
	{
		c_puts("SWI\n");
		c_printf("CU Command complete (%d)\n", eth0.CU_finished);
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_SWI);
	}
	if ((SCB_STAT_ACK_Byte & SCB_STAT_FCP) != 0)
	{
		__outb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK, SCB_STAT_FCP);
	}

	SCB_STAT_ACK_Byte  = __inb(eth0.CSR_BAR + E_CSR_SCB_STAT_ACK);
	SCB_STAT_Byte = __inb(eth0.CSR_BAR + E_CSR_SCB_STAT_WORD);
	c_printf("SCB_STATUS: 0x%02x%02x\n", SCB_STAT_ACK_Byte, SCB_STAT_Byte);

	 __outb( PIC_MASTER_CMD_PORT, PIC_EOI );
	if( vector >= 0x28 && vector <= 0x2f )
	{
		__outb( PIC_SLAVE_CMD_PORT, PIC_EOI );
	}

}
