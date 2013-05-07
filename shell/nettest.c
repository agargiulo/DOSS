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
void nic_wait( void )
{
	uint16_t timeout = 10000;
	uint8_t i = 0;
	while (__inb(CSR_BAR + E_CSR_SCB_COM_WORD)  && --timeout)
	{
		if (i == 0)
		{
			//c_printf("SCB command byte: 0x%02x\n", __inb(CSR_BAR + E_CSR_SCB_COM_WORD));
		}
		i = (i + 1) % 100;
		__delay(2);
	}
}

/*
** PUBLIC FUNCTIONS
*/
void nettest(int argc, char **argv)
{
	// net_pci_dump();
	// net_CSR_dump();
	__outl(CSR_BAR + E_CSR_PORT, 0xFFFFFFF1);

	__delay(100);

	__outl(CSR_BAR + E_CSR_SCB_GEN_PTR, 0x00000000);
	__outb(CSR_BAR + E_CSR_SCB_COM_WORD + 1, 0x02);
	__outb(CSR_BAR + E_CSR_SCB_COM_WORD, 0x60);

	nic_wait();

	 // __outb(CSR_BAR + E_CSR_SCB_COM_WORD, 0x06);
}
