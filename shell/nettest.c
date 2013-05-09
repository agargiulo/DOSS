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
	uint16_t timeout = 10;
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
void run_nettest(int argc, char **argv)
{
	int port = CSR_BAR + E_CSR_SCB_COM_WORD;
	__outl(CSR_BAR + E_CSR_PORT, 0x00000000);
	__delay(100);
	__outl(CSR_BAR + E_CSR_SCB_GEN_PTR, 0x00000000);
	__outb(port + 1, 0x2);
	/*
	nic_wait();
	__outb(port, 0x06);
	nic_wait();
	*/
}
