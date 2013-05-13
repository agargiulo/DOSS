/*
**
** File:	disk.h
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	disk module header file
*/

#ifndef _DISK_H_
#define _DISK_H_

#define __SP2_KERNEL__

#include <common.h>
#include <types.h>

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

/**
 * Initialize the disk. Yup.
 */
void _disk_init( void );

/**
 * Prints the discovered devices
 */
void print_devices( void );

/**
 * Public function for reading data from the current disk
 * @param sector	Sector to read
 * @param buffer	Buffer to read into
 * @param n		Number of bytes to read
 */
status_t disk_read( uint32_t sector, uint8_t *buffer, size_t n );
status_t read_sector( uint32_t sector, uint8_t *buffer );

/**
 * Public function for writing data to the current disk.
 * @param sector	Sector to write
 * @param buffer	Source buffer
 * @param n		Number of bytes to write
 */
status_t disk_write( uint32_t sector, const uint8_t *buffer, size_t n );
status_t write_sector( uint32_t sector, const uint8_t *buffer );


#endif
