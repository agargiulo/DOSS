/*
** SCCS ID:	%W%	%G%
**
** File:	?
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <disk.h>
#include <string.h>

/*
** PRIVATE DEFINITIONS
*/

#define	CLUSTER_SIZE	( 16 * 1024 )
#define	CLUSTER_COUNT	5000
#define	FS_SIZE		( CLUSTER_COUNT * CLUSTER_SIZE )

#define	BOOT_SECTOR	0
#define	SEC_IN_CLUSTER	32
#define	SECTOR_SIZE	512
#define	SIGNATURE	0x4F77656E
/*
** PRIVATE DATA TYPES
*/

typedef struct directory_table_entry {
	char name[112];
	uint32_t index;
	uint32_t size;
	uint32_t type;
	uint32_t time;
} dt_entry_t;


/*
** PRIVATE GLOBAL VARIABLES
*/

uint8_t		fs_initialized = 0;

uint32_t	fat[CLUSTER_COUNT];
dt_entry_t	dir_table[CLUSTER_COUNT];

uint8_t		cluster_buffer[SECTOR_SIZE*SEC_IN_CLUSTER] = {0};

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

void read_cluster( uint32_t index ) {
	int i;
	
	uint32_t sector = index * SEC_IN_CLUSTER;
	
	for ( i = 0; i < SEC_IN_CLUSTER; ++i ) {
		read_sector( sector + i, cluster_buffer+(i*SECTOR_SIZE) );
	}
}

void write_cluster( uint32_t index ) {
	int i;
	
	uint32_t sector = index * SEC_IN_CLUSTER;
	
	for ( i = 0; i < SEC_IN_CLUSTER; ++i ) {
		write_sector( sector + i, cluster_buffer+(i*SECTOR_SIZE) );
	}
}

void clear_buffer( void ) {
	memset( cluster_buffer, 0x00, SECTOR_SIZE*SEC_IN_CLUSTER );
}

#define	CGET8(x)	(*(cluster_buffer+x))
#define	CGET16(x)	(*((uint16_t*)(cluster_buffer+x)))
#define	CGET32(x)	(*((uint32_t*)(cluster_buffer+x)))

// uint8_t cread8( uint32_t offset ) {
// 	return *(cluster_buffer+offset);
// }
// uint16_t cread16( uint32_t offset ) {
// 	return (uint16_t)(*(cluster_buffer+offset));
// }
// uint32_t cread32( uint32_t offset ) {
// 	return (uint32_t)(*(cluster_buffer+offset));
// }
// 
// uint32_t cwrite8( uint8_t val, uint32_t offset ) {
// 	*(cluster_buffer+offset) = val;
// 	return offset+1;
// }
// uint32_t cwrite16( uint16_t val, uint32_t offset ) {
// 	*((uint16_t*)(cluster_buffer+offset)) = val;
// 	return offset+2;
// }
// uint32_t cwrite32( uint32_t val, uint32_t offset ) {
// 	*((uint32_t*)(cluster_buffer+offset)) = val;
// 	return offset+4;
// }

/*
** PUBLIC FUNCTIONS
*/

void _fs_init( void ) {
	
	if ( fs_initialized == 0 ) {
		status_t status;
		
		// Read boot sector
		status = read_sector( BOOT_SECTOR, cluster_buffer );
		if ( status ) return;
		
		if ( CGET32(0) == CLUSTER_SIZE 
			&& CGET32(4) == FS_SIZE 
			&& CGET32(8) == 1
			&& CGET32(12) == 2
			&& CGET32(16) == SIGNATURE ) {
			
			// OFS exists on this device
			
		} else {
			// Initialize the boot sector on this device
			uint32_t fat_index = 1;
			uint32_t dt_index = 2;
			
			CGET32(0) = CLUSTER_SIZE;
			CGET32(4) = FS_SIZE;
			CGET32(8) = dt_index;
			CGET32(12) = fat_index;
			CGET32(16) = SIGNATURE;
			
			// write it to the drive
			
			status = write_sector( BOOT_SECTOR, cluster_buffer );
			if ( status ) return;
			
			
			// Initialize the FAT
			
			
			
		}
		
		fs_initialized = 1;
	}
}