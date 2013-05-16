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
					device_t *d = &device_tab[device_count++];
					d->bus = bus;
					d->slot = slot;
					d->vendor = vendor;
					d->func = func;
					
					d->device = pci_read(bus, slot, func, REG_DEVICE);
					
					d->command = pci_read(bus, slot, func, REG_COMMAND);
					d->status = pci_read(bus, slot, func, REG_STATUS);
					
					d->revid = pci_readb(bus, slot, func, REG_REVID);
					d->progif = pci_readb(bus, slot, func, REG_PROGIF);
					d->class = pci_readb(bus, slot, func, REG_CLASS);
					d->subclass = pci_readb(bus, slot, func, REG_SUBCLASS);
					
					d->clsize = pci_readb(bus, slot, func, REG_CLSIZE);
					d->lattimer = pci_readb(bus, slot, func, REG_LTIMER);
					d->htype = pci_readb(bus, slot, func, REG_HTYPE);
					d->bist = pci_readb(bus, slot, func, REG_BIST);
					
					switch ( d->htype ) {
						case 0:
			d->bar0 = pci_readl(bus, slot, func, REG0_BAR0);
			d->bar1 = pci_readl(bus, slot, func, REG0_BAR1);
			d->bar2 = pci_readl(bus, slot, func, REG0_BAR2);
			d->bar3 = pci_readl(bus, slot, func, REG0_BAR3);
			d->bar4 = pci_readl(bus, slot, func, REG0_BAR4);
			d->bar5 = pci_readl(bus, slot, func, REG0_BAR5);
			
			d->cbcisptr = pci_readl(bus, slot, func, REG0_CBCISPTR);
			
			d->subsysvid = pci_read(bus, slot, func, REG0_SUBVENID);
			d->subsysid = pci_read(bus, slot, func, REG0_SUBID);
			
			d->eprombar = pci_readl(bus, slot, func, REG0_EROMBAR);
			
			d->capptr = pci_readb(bus, slot, func, REG0_CAPPTR);
			
			d->intline = pci_readb(bus, slot, func, REG0_INTLINE);
			d->intpin = pci_readb(bus, slot, func, REG0_INTPIN);
			d->mingrant = pci_readb(bus, slot, func, REG0_MINGRANT);
			d->maxlat = pci_readb(bus, slot, func, REG0_MAXLAT);
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
	ulong_t lslot = (ulong_t) slot;
	ulong_t lfunc = (ulong_t) func;
	ushort_t reg_aligned = (reg & 0xFC); //Guarantees 4-aligned register
	ulong_t enable = (uint32_t) 0x80000000; //Set Enable Bit
	
	address = (lbus << 16) | (lslot << 11) | (lfunc << 8) |
		  (reg_aligned) | (enable);

	return address;
}
