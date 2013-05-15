#ifndef PCI_H
#define PCI_H

// Port Information for Configuring IDE Bus

#define	CONFIG_ADDRESS	0xCF8
#define	CONFIG_DATA	0xCFC

// PCI Class Codes for Supported Devices

#define	MASS_STORAGE	0x01
#define	NETWORK		0x02
#define	DISPLAY		0x03

#define	INVALID		0xFFFF

// Commonly Used Register Offsets (all header types)
// note all types are the same up to 0xF

#define	REG_VENDOR	0x0
#define	REG_DEVICE	0x2
#define	REG_COMMAND	0x4
#define	REG_STATUS	0x6
#define	REG_REVID	0x8
#define	REG_PROGIF	0x9
#define	REG_SUBCLASS	0xA
#define	REG_CLASS	0xB
#define	REG_CLSIZE	0xC
#define	REG_LTIMER	0xD
#define	REG_HTYPE	0xE
#define	REG_BIST	0xF

// Offsets for header type 0x00

#define	REG0_BAR0	0x10
#define	REG0_BAR1	0x14
#define	REG0_BAR2	0x18
#define	REG0_BAR3	0x1C
#define	REG0_BAR4	0x20
#define	REG0_BAR5	0x24
#define	REG0_CBCISPTR	0x28
#define	REG0_SUBVENID	0x2C
#define	REG0_SUBID	0x2E
#define	REG0_EROMBAR	0x30
#define	REG0_CAPPTR	0x34
#define	REG0_RSRVD0	0x35
#define	REG0_RSRVD1	0x38
#define	REG0_INTLINE	0x3C
#define	REG0_INTPIN	0x3D
#define	REG0_MINGRANT	0x3E
#define	REG0_MAXLAT	0x3F

// Offsets for header type 0x01

#define	REG1_BAR0	0x10
#define	REG1_BAR1	0x14
#define	REG1_PBUSNO	0x18
#define	REG1_SBUSNO	0x19
#define	REG1_SUBBUSNO	0x1A
#define	REG1_SLTIMER	0x1B
#define	REG1_IOBASE	0x1C
#define	REG1_IOLIMIT	0x1D
#define	REG1_SSTATUS	0x1E
#define	REG1_MEMBASE	0x20
#define	REG1_MEMLIMIT	0x22
#define	REG1_PREMBASE	0x24
#define	REG1_PREMLIMIT	0x26
#define	REG1_PBASEUP	0x28
#define	REG1_PLIMITUP	0x2C
#define	REG1_IOBASEUP	0x30
#define	REG1_IOLIMITUP	0x32
#define	REG1_CAPPTR	0x34
#define	REG1_RSRVD	0x35
#define	REG1_EXPRBAR	0x38
#define	REG1_INTLINE	0x3C
#define	REG1_INTPIN	0x3D
#define	REG1_BRIDGECTRL	0x3E

// Offsets for header type 0x02

#define	REG2_CBBAR	0x10
#define	REG2_OFFCLIST	0x14
#define	REG2_RSRVD	0x15
#define	REG2_SECSTATUS	0x16
#define	REG2_BUSNO	0x18
#define	REG2_CBNO	0x19
#define	REG2_SOBUSNO	0x1A
#define	REG2_CBLTIMER	0x1B
#define	REG2_MEMBAR0	0x1C
#define	REG2_MEMLIMIT0	0x20
#define	REG2_MEMBAR1	0x24
#define	REG2_MEMLIMIT1	0x28
#define	REG2_IOBAR0	0x2C
#define	REG2_IOLIMIT0	0x30
#define	REG2_IOBAR1	0x34
#define	REG2_IOLIMIT1	0x38
#define	REG2_INTLINE	0x3C
#define	REG2_INTPIN	0x3D
#define	REG2_BRIDGECTRL	0x3C
#define	REG2_SUBDEVID	0x40
#define	REG2_SUBVENID	0x42
#define	REG2_LEGACYADDR	0x44

// Globals
#define	MAX_DEVICES	32

#ifndef __SP2_ASM__

typedef struct device {
	ushort_t bus;
	ushort_t slot;
	ushort_t func;
	
	ushort_t vendor;
	ushort_t device;
	
	ushort_t command;
	ushort_t status;
	
	ubyte_t revid;
	ubyte_t progif;
	ubyte_t class;
	ubyte_t subclass;
	
	ubyte_t clsize;
	ubyte_t lattimer;
	ubyte_t htype;
	ubyte_t bist;
	
	ulong_t bar0;
	ulong_t bar1;
	ulong_t bar2;
	ulong_t bar3;
	ulong_t bar4;
	ulong_t bar5;
	
	ulong_t cbcisptr;
	
	ushort_t subsysvid;
	ushort_t subsysid;
	
	ulong_t eprombar;
	
	ubyte_t capptr;
	
	ubyte_t intline;
	ubyte_t intpin;
	ubyte_t mingrant;
	ubyte_t maxlat;
	
} device_t;

uint8_t device_count;

device_t device_tab[MAX_DEVICES];

// Load the device table

void _pci_init( void );

// PCI Configuration Read Methods

ulong_t pci_readl(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg);
ushort_t pci_read(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg);
ubyte_t pci_readb(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg);

// PCI Configuration Write Methods

void pci_writel(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg, ulong_t data);
void pci_write(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg, ushort_t data);

// Helper Functions

ulong_t pci_getAddress(ushort_t bus, ushort_t slot, ushort_t func, ushort_t reg);

#endif

#endif
