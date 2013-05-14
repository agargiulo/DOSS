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
#include <string.h>
#include <disk.h>
#include <ofs.h>

/*
** PRIVATE DEFINITIONS
*/
#define	SEC_IN_CLUSTER	32
#define	SECTOR_SIZE	512
#define	CLUSTER_SIZE	( SEC_IN_CLUSTER * SECTOR_SIZE )
#define	CLUSTER_COUNT	5000
#define	FS_SIZE		( CLUSTER_COUNT * CLUSTER_SIZE )

#define	BOOT_CLUSTER	0

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

uint32_t	boot_record[CLUSTER_SIZE];
uint32_t	fat[CLUSTER_COUNT];
dt_entry_t	dir_table[CLUSTER_SIZE];

uint8_t		cluster_buffer[CLUSTER_SIZE] = {0};

// uint32_t	fat_indicies[4];
uint8_t		current_fat;
// 
// uint32_t	dt_indicies[4];
uint8_t		current_dt;

uint32_t	dir_table_size;
status_t status;

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

status_t read_cluster( uint32_t index ) {
	int i;
	
	uint32_t sector = index * SEC_IN_CLUSTER;
	
	for ( i = 0; i < SEC_IN_CLUSTER; ++i ) {
		read_sector( sector + i, cluster_buffer+(i*SECTOR_SIZE) );
	}
	
	return 0;
}

status_t write_cluster( uint32_t index ) {
	int i;
	
	uint32_t sector = index * SEC_IN_CLUSTER;
	
	for ( i = 0; i < SEC_IN_CLUSTER; ++i ) {
		write_sector( sector + i, cluster_buffer+(i*SECTOR_SIZE) );
	}
	
	return 0;
}

void clear_buffer( void ) {
	memset( cluster_buffer, 0x00, SECTOR_SIZE*SEC_IN_CLUSTER );
}

status_t update_fat( void ) {
	// Copy fat to our buffer
	memcpy( cluster_buffer, fat, sizeof(fat) );
	// fill the rest with zeroes
	memset( cluster_buffer+sizeof(fat), 0x00, CLUSTER_SIZE-CLUSTER_COUNT );
	
	// write it
	status = write_cluster( current_fat );
	if (status) return status;
		
	clear_buffer();
	return status;
}

status_t load_fat( void ) {
	
	return status;
}

status_t update_dt( void ) {
	// Copy dt to our buffer
	memcpy( cluster_buffer, dir_table, sizeof(dir_table) );
	// fill the rest with zeroes
	//memset( cluster_buffer+sizeof(fat), 0x00, CLUSTER_SIZE-CLUSTER_COUNT );
	
	// write it
	status = write_cluster( current_dt );
	if (status) return status;
		
	clear_buffer();
	return status;
}

status_t load_dt( void ) {
	
	return status;
}

// Shorthand functions for dereferencing values at an offset
#define	CGET8(x)	(*(cluster_buffer+x))
#define	CGET16(x)	(*((uint16_t*)(cluster_buffer+x)))
#define	CGET32(x)	(*((uint32_t*)(cluster_buffer+x)))


/*
** PUBLIC FUNCTIONS
*/

void _fs_init( void ) {
	
	if ( fs_initialized == 0 ) {
		int i;
		
		// Read boot sector
		status = read_sector( BOOT_CLUSTER, cluster_buffer );
		if ( status ) return;
		
		if ( CGET32(0) == CLUSTER_SIZE 
			&& CGET32(4) == FS_SIZE 
			&& CGET32(8) == 1
			&& CGET32(12) == 2
			&& CGET32(16) == SIGNATURE ) {
			
			c_printf("ofs found, loading contents\n");
			current_dt = CGET32(8);
			current_fat = CGET32(12);
			
			// OFS exists on this device
			
			
			// Load fat
			// Load dt
		} else {
			// Initialize the boot sector on this device
			
			CGET32(0) = CLUSTER_SIZE;
			CGET32(4) = FS_SIZE;
			CGET32(8) = current_dt;
			CGET32(12) = current_fat;
			CGET32(16) = SIGNATURE;
			
			// write it to the drive
			
			status = write_cluster( BOOT_CLUSTER );
			if ( status ) {
				// handle error
				return;
			}
			
			clear_buffer();
			
			// Initialize DT
			dir_table_size = CLUSTER_SIZE/sizeof(dt_entry_t);
			for ( i = 0; i < dir_table_size; ++i ) {
				dir_table[i].name[0] = 0;
			}
			
			// Initialize the FAT
			memset( fat, 0x00, CLUSTER_COUNT );
			memset( fat, 0xFF, 3*sizeof(uint32_t) );
			
			// Update FAT
			status = update_fat();
			if (status) {
				// handle error
				return;
			}
			
			
			
			
		}
		
		fs_initialized = 1;
	}
}

file_t * fopen( char * name ) {
	return 0;
}

int fclose( file_t * file ) {
	return 0;
}

int fseek( file_t * file, uint32_t offset ) {
	return 0;
}

size_t fread( void * buffer, size_t size, size_t count, file_t * file ) {
	return 0;
}

size_t fwrite( const void * buffer, size_t size, size_t count, file_t * file ) {
	return 0;
}