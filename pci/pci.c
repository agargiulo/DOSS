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

ulong_t pci_readl(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off)
{
	return 0xFFFF
}

ulong_t pci_read(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off)
{
	return 0xFFFF
}

ulong_t pci_readb(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off)
{
	return 0xFFFF;
}

void pci_writel(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off, ulong_t dat)
{
}

void pci_write(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off, ushort_t dat)
{
}

//Information from wiki.osdev.org/PCI
ulong_t pci_getAddress(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off)
{
	ulong_t address;
	ulong_t lbus = (ulong_t) bus;
	ulong_t lslot = (ulong_t) lslot;
	ulong_t lfunc = (ulong_t) function;
	ushort_t off_aligned = (offset & 0xFC); //Guarantees 4-aligned register
	ulong_t enable = (uint32_t) 0x80000000; //Set Enable Bit
	
	address = (lbus << 16) | (lslot << 11) | (lfunc << 8) |
		  (off_aligned) | (enable);

	return address;
}
