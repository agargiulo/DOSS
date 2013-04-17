/*
** File:	pci.c
** Author:	Alex Walcutt
**
** Description: Handles the configuration, enumeration, and communication of
**		devices on the PCI bus
**
*/

#include <common.h>
#include <pci.h>
#include <startup.h>

void _pci_init( void )
{
	ushort_t bus;
	ushort_t slot;
	ushort_t func;

	ushort_t vendor;
	ushort_t device;
	ubyte_t class;
	ubyte_t subclass;

	c_puts(" pci");
	
	device_count = 0;
	
	for(bus = 0; bus < 256; bus++)
	{
		for(slot = 0; slot < 32; slot++)
		{
			for(func = 0; func < 8; func++)
			{
				vendor = pci_read(bus, slot, func, REG_VENDOR);
				if(vendor != INVALID)
				{
					device = pci_read(bus, slot, func, REG_DEVICE);
					class = pci_readb(bus, slot, func, REG_CLASS);
					subclass = pci_readb(bus, slot, func, REG_SUBCLASS);
					
					device_t d = { bus, slot, func, vendor, device, class, subclass };
					device_tab[device_count++] = d;
					
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
	ulong_t lslot = (ulong_t) slot;
	ulong_t lfunc = (ulong_t) func;
	ushort_t reg_aligned = (reg & 0xFC); //Guarantees 4-aligned register
	ulong_t enable = (uint32_t) 0x80000000; //Set Enable Bit
	
	address = (lbus << 16) | (lslot << 11) | (lfunc << 8) |
		  (reg_aligned) | (enable);

	return address;
}
