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
		c_printf("i: %d\n", i);
		i = (i + 1) % 100;
		__delay(2);
	}
}

/*
** PUBLIC FUNCTIONS
*/
void run_nettest(int argc, char **argv)
{
	/*
	nic_wait();
	__outb(port, 0x06);
	nic_wait();
	*/
}
