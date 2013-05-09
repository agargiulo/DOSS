/*
** File:	disk.c
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	Handle the IDE device initialization, provide system accessible 
** functions for writing to the disk.
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <disk.h>
#include <pci.h>
#include <startup.h>
#include <support.h>
#include <ulib.h>
#include <x86arch.h>


/*
** PRIVATE DEFINITIONS
*/

#define	IDE_ATA		0
#define	IDE_ATAPI	1

#define	ATA_MASTER	0
#define	ATA_SLAVE	1

#define	ATA_PRIMARY	0
#define	ATA_SECONDARY	1

// Limits
#define	MAX_CONTROLLERS		4
#define	MAX_CHANNELS		8
#define	MAX_IDE_DEVICES		16

/*
** PRIVATE DATA TYPES
*/

typedef struct ide_controller {
	device_t		*device;	// Base device info
	struct ide_channel	*channel;
	struct ide_device	*idevice;
} ide_controller_t;

struct ide_channel {
	uint32_t		base;		// I/O Base.
	uint32_t		ctrl;		// Control Base
	uint32_t		bmide; 		// Bus Master IDE
	uint8_t			int_line;
} channels[2];

struct ide_device {
	uint8_t		reserved;	// 0 (Empty) or 1 (This Drive really exists).
	uint8_t		channel;	// Channel 0 or 1
	uint8_t		drive;		// 0 (Master Drive) or 1 (Slave Drive).
	uint16_t	type;		// 0: ATA, 1:ATAPI.
	uint16_t	signature;	// Drive Signature
	uint16_t	capabilities;	// Features.
	uint32_t	commandSets;	// Command Sets Supported.
	uint32_t	size;		// Size in Sectors.
	uint8_t		model[41];	// Model in string.
} ide_devices[4];

/*
** PRIVATE GLOBAL VARIABLES
*/

uint8_t ide_device_count;

uint8_t ide_buffer[2048];

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

void ide_isr( int vec, int code ) {
	
	
// 	c_printf("isr %x\n", vec );
	if ( vec == 0x2e )
		__outb( PIC_MASTER_CMD_PORT, PIC_EOI );
	else if ( vec == 0x23 )
		__outb( PIC_SLAVE_CMD_PORT, PIC_EOI );
}

void ide_write( uint8_t channel, uint8_t reg, const uint8_t data ) {
// 	c_printf("writing %x %x\n", channels[channel].base + reg, data);
	
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 0x82);
	if (reg < 0x08)
		__outb(channels[channel].base + reg - 0x00, data);
	else if (reg < 0x0C)
		__outb(channels[channel].base + reg - 0x06, data);
	else if (reg < 0x0E)
		__outb(channels[channel].base + reg - 0x0A, data);
	else if (reg < 0x16)
		__outb(channels[channel].bmide + reg - 0x0E, data);
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 2);
// 	c_printf("done write\n");
}

uint8_t ide_read( uint8_t channel, uint8_t reg ) {
	uint8_t result;
	//c_printf("reading %x\n", channels[channel].base + reg);
	
	
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 0x82);
	if (reg < 0x08)
		result = __inb(channels[channel].base + reg - 0x00);
	else if (reg < 0x0C)
		result = __inb(channels[channel].base + reg - 0x06);
	else if (reg < 0x0E)
		result = __inb(channels[channel].base + reg - 0x0A);
	else if (reg < 0x16)
		result = __inb(channels[channel].bmide + reg - 0x0E);
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 2);
	
	//c_printf("read %x %x\n", channels[channel].base + reg, result);
	
	return result;
}


uint8_t ide_polling(uint8_t channel, uint32_t advanced_check) {
 
   // (I) Delay 400 nanosecond for BSY to be set:
   // -------------------------------------------------
   for(int i = 0; i < 4; i++)
      ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.
 
   // (II) Wait for BSY to be cleared:
   // -------------------------------------------------
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
      ; // Wait for BSY to be zero.
 
   if (advanced_check) {
      uint8_t state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.
 
      // (III) Check For Errors:
      // -------------------------------------------------
      if (state & ATA_SR_ERR)
         return 2; // Error.
 
      // (IV) Check If Device fault:
      // -------------------------------------------------
      if (state & ATA_SR_DF)
         return 1; // Device Fault.
 
      // (V) Check DRQ:
      // -------------------------------------------------
      // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
      if ((state & ATA_SR_DRQ) == 0)
         return 3; // DRQ should be set
 
   }
 
   return 0; // No Error.
 
}

void pio_lba28_write( struct ide_device *device, uint32_t sector, const uint8_t *buffer, size_t n ) {
	uint8_t status;
	int i;
	
// 	turn off interrupts
	ide_write( device->channel, ATA_REG_CONTROL, 2 );
	
	//wait for the drive to not be busy
	
	while ( 1 ) {
		status = ide_read( device->channel, ATA_REG_STATUS );
		if ( !(status & ATA_SR_BSY) ) break;
	}
	
	// Select
	ide_write( device->channel, ATA_REG_HDDEVSEL, 0xE0 | (device->drive << 4) | ((sector >> 24) & 0x0F) );
	
	// write lba data to upper, then lower
	ide_write( device->channel, 0x1, 0 );
	ide_write( device->channel, ATA_REG_SECCOUNT0, 1 );
// 	ide_write( device->channel, ATA_REG_LBA3, ( sector & 0xFF000000 ) >> 24 );
// 	ide_write( device->channel, ATA_REG_LBA4, 0 );
// 	ide_write( device->channel, ATA_REG_LBA5, 0 );
// 	ide_write( device->channel, ATA_REG_SECCOUNT0, 1 );
	ide_write( device->channel, ATA_REG_LBA0, (uint8_t) sector );
	ide_write( device->channel, ATA_REG_LBA1, (uint8_t) (sector >> 8) );
	ide_write( device->channel, ATA_REG_LBA2, (uint8_t) (sector >> 16) );
	
	
	
	// send command
	ide_write( device->channel, ATA_REG_COMMAND, CMD_WRITE_SECTORS_EXT );
	
	uint16_t *buf = (uint16_t*)buffer;
	
 	status = ide_polling( device->channel, 1 );
	while ( !(ide_read( device->channel, ATA_REG_STATUS) & 0x08) );
 	if ( !status ) {
// 		c_printf("writing data\n");
		int towrite = ((n/2)+1);
		int zeroes = 256 - towrite;
		for ( i = 0; i < towrite; ++i ) {
			__outw( channels[device->channel].base, buf[i] );
		}
		
		for ( i = 0; i < zeroes; ++i ) {
			__outw( channels[device->channel].base, 0 );
		}
 	}
 	else {
		c_printf("device busy\n");
	}
	
}

void pio_lba28_read( struct ide_device *device, uint32_t sector, uint8_t *buffer, size_t n ) {
	uint8_t status;
	int i;
	
	// turn off interrupts
	ide_write( device->channel, ATA_REG_CONTROL, 2 );
	
	//wait for the drive to not be busy
	
	while ( 1 ) {
		status = ide_read( device->channel, ATA_REG_STATUS );
		if ( !(status & ATA_SR_BSY) ) break;
	}
	
	// select
	ide_write( device->channel, ATA_REG_HDDEVSEL, 0xE0 | (device->drive << 4) | ((sector >> 24) & 0x0F) );
	
	// write lba data to upper, then lower
	ide_write( device->channel, ATA_REG_FEATURES, 0 );
	ide_write( device->channel, ATA_REG_SECCOUNT0, 1 );
//	ide_write( device->channel, ATA_REG_LBA3, ( sector & 0xFF000000 ) >> 24 );
//	ide_write( device->channel, ATA_REG_LBA4, 0 );
//	ide_write( device->channel, ATA_REG_LBA5, 0 );
//	ide_write( device->channel, ATA_REG_SECCOUNT0, 1 );
	ide_write( device->channel, ATA_REG_LBA0, (uint8_t) sector );
	ide_write( device->channel, ATA_REG_LBA1, (uint8_t) (sector >> 8) );
	ide_write( device->channel, ATA_REG_LBA2, (uint8_t) (sector >> 16) );
	
	
	// send command
	ide_write( device->channel, ATA_REG_COMMAND, CMD_READ_SECTORS_EXT );
	
	status = ide_polling( device->channel, 1 );
	while ( !(ide_read( device->channel, ATA_REG_STATUS) & 0x08) );
	if ( !status ) {
// 		c_printf("reading data\n");
		uint16_t temp;
		int towrite = ((n/2)+1);
		int zeroes = 256 - towrite;
		for ( i = 0; i < towrite; ++i ) {
			temp = __inw( channels[device->channel].base );
			(buffer)[2*i] = (uint8_t)temp;
			(buffer)[(2*i)+1] = (uint8_t)(temp >> 8);
			//c_printf("%x ", temp);
		}
		for ( i = 0; i < zeroes; ++i ) {
			__inw( channels[device->channel].base );
		}
 	}
 	else {
		c_printf("device busy");
	}
	
}



void ide_read_buffer8( uint8_t channel, uint8_t reg, uint8_t *buffer, size_t n ) {
	int i;
	
	for(i = 0; i < n; ++i) {
		buffer[i] = __inb(channels[channel].base);	
	}
}

void ide_read_buffer16( uint8_t channel, uint8_t reg, uint16_t *buffer, size_t n ) {
	int i;
	
	for(i = 0; i < n; ++i) {
		buffer[i] = __inw(channels[channel].base);	
	}
}

void ide_read_buffer32( uint8_t channel, uint8_t reg, uint32_t *buffer, size_t n ) {
	int i;
	
// 	if (reg > 0x07 && reg < 0x0C)
// 		ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].int_line);
	
	for(i = 0; i < n; ++i) {
		buffer[i] = __inl(channels[channel].base);	
	}
	
// 	if (reg > 0x07 && reg < 0x0C)
// 		ide_write(channel, ATA_REG_CONTROL, channels[channel].int_line);
}

/*
** PUBLIC FUNCTIONS
*/



void _disk_init( void ) {
	int i, j, k, m;
	
	// Initialize counters
	ide_device_count = 0;
	__install_isr( 0x2E, ide_isr ); // pata controller
	__install_isr( 0x23, ide_isr ); // sata controller
	c_printf(" ide");
	// Look for IDE controllers in device tab
	for ( i = 0; i < device_count; ++i ) {
		device_t *dev = &device_tab[i];
		// If it's an IDE controller
		if ( dev->class == MASS_STORAGE ) {
// 			c_printf("disk found\n");
			
			channels[ATA_PRIMARY].base = 0x1F0; //f0e0 //(dev->bar0 & 0xFFFFFFFC) + 0x1F0 * (!(dev->bar0));
			channels[ATA_PRIMARY].ctrl = 0x3F4; //f0d0 //(dev->bar1 & 0xFFFFFFFC) + 0x3F4 * (!(dev->bar1));
			channels[ATA_SECONDARY].base = 0x170; //f0c0 //(dev->bar2 & 0xFFFFFFFC) + 0x170 * (!(dev->bar2));
			channels[ATA_SECONDARY].ctrl = 0x374; //f0b0 //(dev->bar3 & 0xFFFFFFFC) + 0x374 * (!(dev->bar3));
			channels[ATA_PRIMARY].bmide = 0xf0f0; // //(dev->bar4 & 0xFFFFFFFC) + 0; // Bus Master IDE
			channels[ATA_SECONDARY].bmide = 0xf0f8; //(dev->bar4 & 0xFFFFFFFC) + 8; // Bus Master IDE
			
			
			ide_write(ATA_PRIMARY, ATA_REG_CONTROL, 2);
			ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);
			
			// Go through channels
			for ( j = 0; j < 2; ++j ) {
			// Check devices on this channel
			for ( k = 0; k < 2; ++k ) {
// 				c_printf("k loop\n");
				uint8_t error = 0;
				uint8_t type = IDE_ATA;
				uint8_t status;
				
				ide_devices[ide_device_count].reserved = 0;
// 				c_printf("1\n");
				// Select drive
				ide_write(j, ATA_REG_HDDEVSEL, 0xA0 | ( k << 4 ));
				
				__delay(1);
// 				c_printf("2\n");
				// Send identify command
				ide_write(j, ATA_REG_COMMAND, CMD_IDENTIFY_DEVICE);
				
				__delay(1);
// 				c_printf("3\n");
				// if status is 0, no device
				if ( ide_read( j, ATA_REG_STATUS ) == 0 ) {
					continue;
				}
// 				c_printf("device exists %d %d\n", j, k );
				while ( 1 ) {
					status = ide_read( j, ATA_REG_STATUS );
					
					if ( (status & ATA_SR_ERR) ) {
						// Bad
						error = 1;
						break;
					}
					if ( !(status & ATA_SR_BSY) &&
						(status & ATA_SR_DRQ) ) {
						// Good
						break;
					}
				}
				
				if ( error != 0 ) {
					continue;
					uint8_t low = ide_read(j, ATA_REG_LBA1);
					uint8_t high = ide_read(j, ATA_REG_LBA2);
					
					if ( low == 0x14 && high == 0xEB ) {
						type = IDE_ATAPI;
					} else if ( low == 0x69 && high == 0x96 ) {
						type = IDE_ATAPI;
					}
					else continue; // unknown
					
					ide_write(j, ATA_REG_COMMAND, CMD_IDENTIFY_DEVICE_PACKET);
					__delay(1);
				}
				
				ide_read_buffer32( j, ATA_REG_DATA, (uint32_t*)ide_buffer, 128 );
				
				// Drive is good, setting data
				ide_devices[ide_device_count].reserved 	= 1;
				ide_devices[ide_device_count].type 		= type;
				ide_devices[ide_device_count].channel	 	= j;
				ide_devices[ide_device_count].drive		= k;
				ide_devices[ide_device_count].signature	= *((uint16_t*)(ide_buffer+ATA_IDENT_DEVICETYPE));
				ide_devices[ide_device_count].capabilities	= *((uint16_t*)(ide_buffer+ATA_IDENT_CAPABILITIES));
				ide_devices[ide_device_count].commandSets	= *((uint32_t*)(ide_buffer+ATA_IDENT_COMMANDSETS));
				
				if ( ide_devices[ide_device_count].commandSets & (1 << 26) ) {
					// 48 bit addressing
					ide_devices[ide_device_count].size = 
						*((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA_EXT));
				} else {
					// chs or 28 bit addressing
					ide_devices[ide_device_count].size = 
						*((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA));
				}
				
				for ( m = 0; m < 40; m+=2 ) {
					ide_devices[ide_device_count].model[m] = ide_buffer[ATA_IDENT_MODEL + m + 1];
					ide_devices[ide_device_count].model[m+1] = ide_buffer[ATA_IDENT_MODEL + m];
				}
				
				ide_devices[ide_device_count].model[40] = 0;
				
				++ide_device_count;
				
			} 
			// End channel->device check
				
			} 
			// End channel check
			break; // Stop looking after PATA controller found
		}
		// else not storage, ignore
	}
	// Done devices
// 	c_printf("done disk\n");
	
}

void print_devices() {
	int i;
	
	for (i = 0; i < 4; i++) {
		if (ide_devices[i].reserved == 1) {
			c_printf(" Found %s Drive %dGB - %s\n",
				(const char *[]){"ATA", "ATAPI"}[ide_devices[i].type],
				ide_devices[i].size / 1024 / 1024 / 2,
				ide_devices[i].model);
		}
	}
}

status_t disk_read( uint32_t sector, uint8_t *buffer, size_t n ) {
	pio_lba28_read( &ide_devices[0], sector, buffer, n );
	return 0;
}

// status_t disk_read( uint64_t start, uint64_t stop, uint8_t *buffer ) {
// 	return 0;
// }

status_t disk_write( uint32_t sector, const uint8_t *buffer, size_t n ) {
	pio_lba28_write( &ide_devices[0], sector, buffer, n );
	return 0;
}
