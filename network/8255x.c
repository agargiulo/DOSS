/*
 * File:	?
 *
 * Author:	4003-506 class of 20123
 *
 * Contributor:
 *
 * Description:	?
 */

#define	__SP2_KERNEL__

#include <common.h>
#include <pci.h>
#include <8255x.h>

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE DATA TYPES
 */
device_t *my_devices[3];

/*
 * PRIVATE GLOBAL VARIABLES
 */

/*
 * PUBLIC GLOBAL VARIABLES
 */

/*
 * PRIVATE FUNCTIONS
 */

/*
 * PUBLIC FUNCTIONS
 */

void _net_init(void)
{
	c_puts(" network\n");
	ushort_t status_reg;
	int dev_index = 0;
	for (int i = 0; i < device_count; i++)
	{
		if (device_tab[i].device == 0x1229)
		{
			my_devices[dev_index++] = &device_tab[i];
			status_reg = pci_readb(device_tab[i].bus, device_tab[i].slot,
					device_tab[i].func, P_ETH_STATUS_REG);
			c_printf("status: 0x%04x\n", status_reg);
		}
	}
}
