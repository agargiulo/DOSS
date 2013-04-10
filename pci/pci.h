#ifndef PCI_H
#define PCI_H

// Port Information for Configuring IDE Bus

#define	CONFIG_ADDRESS	0xCF8
#define CONFIG_DATA	0xCFC

// IDE Class Codes for Supported Devices

#define MASS_STORAGE	0x01
#define NETWORK		0x02
#define DISPLAY		0x03

#define INVALID		0xFFFF

void pci_init( void );

// PCI Configuration Read Methods

ulong_t pci_readl(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off);
ushort_t pci_read(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off);
ubyte_t pci_readb(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off);

// PCI Configuration Write Methods

void pci_writel(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off, ulong_t dat);
void pci_write(ushort_t bus, ushort_t slot, ushort_t func, ushort_t off, ulong_t dat);

// Helper Functions

ulong_t pci_getAddress(ushort_t bus, ushort_t slot, ushort_t function, ushort_t offset);
