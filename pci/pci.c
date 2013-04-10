/*
** File:	pci.c
** Author:	Alex Walcutt
**
** Description: Handles the configuration, enumeration, and communication of
**		devices on the PCI bus
**
*/

#include "pci.h"

void _pci_init( void )
{
	ulong_t address;

	ushort_t bus;
	ushort_t slot;
	ushort_t func;
	ushort_t reg;

	ushort_t vendor;
	ushort_t device;
	ubyte_t class;
	ubyte_t subclass;

	c_puts("PCI\n");

	for(bus = 0; bus < 256; bus++)
	{
		for(slot = 0; slot < 256; slot++)
		{
			for(func = 0; func < 8; func++)
			{
				vendor = pci_read(bus, slot, func, REG_VENDOR);
				if(vendor != INVALID)
				{
					class = pci_readb(bus, slot, func, REG_CLASS);
					subclass = pci_readb(bus, slot, func, REG_SUBCLASS);

					switch(class)
					{
						case (MASS_STORAGE):
							c_puts("MASS STORAGE DEVICE FOUND\n");
							break;
						case (NETWORK):
							c_puts("NETWORK DEVICE FOUND\n");
							break;
						case (DISPLAY):
							c_puts("DISPLAY DEVICE FOUND\n");
							break;
						default:
							//Unsupported Device
							break;
					}
				}
			}
		}
	}
}

ulong_t pci_readl(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg)
{
	ulong_t address = pci_getAddress(bus, slot, func, reg);
	ulong_t data;
	
	__outl(CONFIG_ADDRESS, address);
	data = __inl(CONFIG_DATA);

	return data;
}

ushort_t pci_read(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg)
{
	ulong_t address = pci_getAddress(bus, slot, func, reg);
	ushort_t data;

	__outl(CONFIG_ADDRESS, address);
	//Magical bit shifting from OSDev Wiki to get the requested word
	data = (ushort_t) ((__inl(CONFIG_DATA) >> ((reg & 2) * 8)) & 0xffff);

	return data;
}

ubyte_t pci_readb(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg)
{
	ushort_t word = pci_read(bus, slot, func, reg);
	ubyte_t data;

	if(reg % 2 == 0) data = (word & 0xFF);
	else data = (word >> 8);

	return data;
}

void pci_writel(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg, ulong_t data)
{
	ulong_t address = pci_getAddress(bus, slot, func, reg);

	__outl(CONFIG_ADDRESS, address);
	__outl(CONFIG_DATA, data);
}

void pci_write(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg, ushort_t data)
{
	ulong_t address = pci_getAddress(bus, slot, func, reg);

	__outl(CONFIG_ADDRESS, address);
	__outw(CONFIG_DATA, data);
}

//Information from wiki.osdev.org/PCI
ulong_t pci_getAddress(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg)
{
	ulong_t address;
	ulong_t lbus = (ulong_t) bus;
	ulong_t lslot = (ulong_t) lslot;
	ulong_t lfunc = (ulong_t) function;
	ushort_t reg_aligned = (regset & 0xFC); //Guarantees 4-aligned register
	ulong_t enable = (uint32_t) 0x80000000; //Set Enable Bit
	
	address = (lbus << 16) | (lslot << 11) | (lfunc << 8) |
		  (reg_aligned) | (enable);

	return address;
}
