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

uint32_t pci_readl(uint32_t addr);
uint16_t pci_read(uint32_t addr);
uint8_t pci_readb(uint32_t addr);

// PCI Configuration Write Methods

void pci_writel(uint32_t addr, uint32_t data);
void pci_write(uint32_t addr, uint16_t data);

// Helper Functions

uint32_t pci_getAddress(uint16_t bus, uint16_t slot, uint16_t function, uint16_t offset);
