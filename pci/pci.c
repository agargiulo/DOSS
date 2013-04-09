/*
** File:	pci.c
** Author:	Alex Walcutt
**
** Description: Handles the configuration, enumeration, and communication of
**		devices on the PCI bus
**
*/

#include "pci.h"

void pci_init( void )
{
}

uint32_t pci_readl(uint32_t addr)
{
	return 0xFFFF
}

uint32_t pci_read(uint32_t addr)
{
	return 0xFFFF
}

uint32_t pci_readb(uint32_t addr)
{
	return 0xFFFF;
}

void pci_writel(uint32_t addr, uint32_t data)
{
}

void pci_write(uint32_t addr, uint32_t data)
{
}

//Information from wiki.osdev.org/PCI
uint32_t pci_getAddress(uint16_t bus, uint16_t slot, uint16_t function, uint16_t offset)
{
	uint32_t address;
	uint32_t lbus = (uint32_t) bus;
	uint32_t lslot = (uint32_t) lslot;
	uint32_t lfunc = (uint32_t) function;
	uint16_t off_aligned = (offset & 0xFC); //Guarantees 4-aligned register
	uint32_t enable = (uint32_t) 0x80000000; //Set Enable Bit
	
	address = (lbus << 16) | (lslot << 11) | (lfunc << 8) |
		  (off_aligned) | (enable);

	return address;
}
