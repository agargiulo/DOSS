/*
<<<<<<< HEAD
 * File: network/8255x.c
 *
 * Author: Anthony Gargiulo (20123)
 *
 * Description: Driver for the Intel Pro 100 8255x (rev 8) card
=======
 * File:	?
 *
 * Author:	4003-506 class of 20123
 *
 * Contributor:
 *
 * Description:	?
>>>>>>> Basic things for the 8255x card, and a few other minor changes
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
device_t *eth0;

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
	// Look through the device_tab for the network card
	// In this case, the 8255x (device id 0x1229)
	for (int i = 0; i < device_count; i++)
	{
		if (device_tab[i].device == 0x1229)
		{
			eth0 = &device_tab[i];
		}
	}
	c_puts(" network\n");
}


void net_pci_dump(void)
{
	c_printf("device ID: 0x%04x    vendor ID: 0x%04x\n", eth0->device, eth0->vendor);
	c_printf(" Status  : 0x%04x    Command  : 0x%04x\n",
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_STATUS_REG),
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_CMD_REG));
	c_printf(" Class   : 0x%04x00     Rev ID  : 0x%02x\n",
			eth0->class,
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_REV_ID));
	c_printf("BIST: 0x%02x    Head Type: 0x%02x    Lat Timer: 0x%02x    Cache Line Size: 0x%02x\n",
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_BIST),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_HEADER_TYPE),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_LAT_TIMER),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_CACHE_LINE_SIZE));
	c_printf("CSR Mem Mapped Base Address Register: 0x%08x\n",
			pci_readl(eth0->bus, eth0->slot, eth0->func, P_ETH_CSR_MEM_MAP_BAR));
	c_printf("CSR I/O Mapped Base Address Register: 0x%08x\n",
			pci_readl(eth0->bus, eth0->slot, eth0->func, P_ETH_CSR_IO_MAP_BAR));
	c_printf("Flash Mem Mapped Base Address Register: 0x%08x\n",
			pci_readl(eth0->bus, eth0->slot, eth0->func, P_ETH_FLASH_MEM_MAP_BAR));
	c_printf("Subsystem ID: 0x%04x    Subsystem Vendor ID: 0x%04x\n",
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_SUBSYS_ID),
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_SUBSYS_VEND_ID));
	c_printf("Expanson ROM Base Address Register: 0x%08x\n",
			pci_readl(eth0->bus, eth0->slot, eth0->func, P_ETH_EX_ROM_BAR));
	c_printf("Cap_Ptr: 0x%02x\n",
			pci_readb(eth0->bus, eth0->slot, eth0->func, P_ETH_CAP_PTR));
	c_printf("Max Latency: 0x%02x    Min Grant: 0x%02x    Inter Pin: 0x%02x    Inter Line: 0x%02x\n",
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_MAX_LATENCY),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_MIN_GRANT),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_INT_PIN),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_INT_LINE));
	c_printf("Pow Man Cap: 0x%04x    Next Item Prt: 0x%02x    Cap ID: 0x%02x\n",
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_POW_MAN_CAP),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_NXT_ITM_PTR),
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_CAP_ID));
	c_printf("Data: 0x%02x    Pow Man CSR: 0x%04x\n",
			pci_readb(eth0->bus,eth0->slot,eth0->func, P_ETH_DATA),
			pci_read(eth0->bus,eth0->slot,eth0->func, P_ETH_POW_MAN_CSR));
}
