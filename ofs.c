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
#include <qalloc.h>
#include <clock.h>

/*
** PRIVATE DEFINITIONS
*/
#define	SEC_IN_CLUSTER	32
#define	SECTOR_SIZE	512
#define	CLUSTER_SIZE	( SEC_IN_CLUSTER * SECTOR_SIZE )
#define	CLUSTER_COUNT	CLUSTER_SIZE
#define	FS_SIZE		( CLUSTER_COUNT * CLUSTER_SIZE )

#define DIR_TABLE_SIZE ( CLUSTER_SIZE/sizeof(dt_entry_t) )

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

// Copies of disk data (first 3 clusters)
uint32_t	boot_record[CLUSTER_SIZE];
uint32_t	fat[CLUSTER_COUNT];
dt_entry_t	dir_table[CLUSTER_SIZE];

// Global buffer for cluster operations
uint8_t		cluster_buffer[CLUSTER_SIZE];

static uint32_t		current_fat;
static uint32_t		current_dt;

// Global status 
static status_t		status;


/*
** PUBLIC GLOBAL VARIABLES
*/
#define	MAXFILES	32
file_t		fd[MAXFILES];

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
	memset( cluster_buffer, 0x00, CLUSTER_SIZE );
}

// Quick function for rewriting the FAT to disk. Clears the global buffer.
status_t update_fat( void ) {
	// Copy fat to our buffer
	memcpy( cluster_buffer, fat, CLUSTER_COUNT );
	
	// write it
	status = write_cluster( current_fat );
	if (status) {
		c_printf("Error updating FAT\n");
		return status;
	}
		
	clear_buffer();
	
	
	return status;
}

// Reload the fat from disk
status_t load_fat( void ) {
	status = read_cluster( current_fat );
	if (status) {
		c_printf("Error loading FAT\n");
		return status;
	}
	memcpy( fat, cluster_buffer, CLUSTER_COUNT );
	return status;
}

// Function for rewriting the Directory Table to disk. Clears the global buffer.
status_t update_dt( void ) {
	// Copy dt to our buffer
	memcpy( cluster_buffer, dir_table, CLUSTER_SIZE );
	
	// write it
	status = write_cluster( current_dt );
	if (status) {
		c_printf("Error updating DT\n");
		return status;
	}
	
	clear_buffer();
	return status;
}

// Reload the directory table from the disk
status_t load_dt( void ) {
	status = read_cluster( current_dt );
	if (status) {
		c_printf("Error loading DT\n");
		return status;
	}
	memcpy( dir_table, cluster_buffer, CLUSTER_SIZE );
	return status;
}

int get_free_fd( void ) {
	int i;
	for ( i = 0; i < MAXFILES; ++i ) {
		if ( fd[i].name[0] == 0x00 ) 
			return i;
	}
	
	c_printf("Error: maximum open files\n");
	return -1;
}

int find_in_dt( char * filename ) {
	int i;
	
	for ( i = 0; i < DIR_TABLE_SIZE; ++i ) {
		if ( strcmp( dir_table[i].name, filename ) == 0 )
			return i;
	}
// 	c_printf("File not found: %s\n");
	return -1;
}

int get_free_dti( void ) {
	int i;
	
	for ( i = 0; i < DIR_TABLE_SIZE; ++i ) {
		//c_printf("dt: %x\n",dir_table[i].name[0]);
		if ( dir_table[i].name[0] == 0x00 ||
			dir_table[i].name[0] == 0xFF )
			return i;
	}
	
	c_printf("Error: DT is full\n");
	return -1;
}

int get_free_fat( void ) {
	int i;
	
	for( i = 0; i < CLUSTER_SIZE; ++i ) {
		if ( fat[i] == 0x00000000 ) {
			return i;
		}
	}
	
	c_printf("Error: FAT is full\n");
	return -1;
}

void clear_fat_chain( uint32_t index ) {
	clear_buffer();
	
	uint32_t current = index;
	uint32_t next = fat[current];
	
	while( next != 0xFFFFFFFF ) {	
		
		write_cluster( current );
		fat[current] = 0;
		current = next;
		next = fat[current];
	}
	
	write_cluster( current );
	fat[current] = 0;
}

int read_data( uint32_t cluster_index, uint8_t** buffer, size_t size ) {
    int dsize = CLUSTER_SIZE;
    uint8_t *data = qalloc(dsize);
    uint8_t *currentPos = data;
    int i = 0;
    
    while (cluster_index != 0xFFFFFFFF) {
    
        if ( i > 0 ) {
            int newsize = CLUSTER_SIZE*(i+1);
            uint8_t *temp = qalloc( newsize );
            memcpy( temp, data, dsize );
            
            //qfree( data, dsize );
            dsize = newsize;
            data = temp;
        }
        ++i;
        
        read_cluster( cluster_index );
	memcpy( currentPos, cluster_buffer, CLUSTER_SIZE );
	currentPos += CLUSTER_SIZE;
	
        cluster_index = fat[cluster_index];
    }
    
    memcpy(*buffer, data, size);
    
    //qfree(data, dsize);
    
    return CLUSTER_SIZE*i;
}

void write_data( uint32_t first_cluster, uint8_t *data, size_t size ) {
	uint8_t *currentPos = data;
	size_t writeSize = size;
	size_t leftToWrite = size;
	
	if (CLUSTER_SIZE < size) {
		writeSize = CLUSTER_SIZE;
	}
// 	c_printf("writing %d\n", writeSize);
	memcpy( cluster_buffer, currentPos, writeSize );
	if ( writeSize < CLUSTER_SIZE ) {
		memset( cluster_buffer + writeSize, 0x00, CLUSTER_SIZE-writeSize );
	}
	write_cluster( first_cluster );
// 	c_printf("buffer contents before write: %x%x%x%x%x\n", 
// 		 data[0], data[1], data[2],
// 		 data[3], data[4]
// 		);
	leftToWrite -= writeSize;
	
	int thisCluster = first_cluster;
	int nextCluster = 0;
	
	while ( leftToWrite > 0 ) {
		currentPos += writeSize;
		
		if (writeSize > leftToWrite) {
			writeSize = leftToWrite;
		}
		
		nextCluster = get_free_fat();
		fat[thisCluster] = nextCluster;
		
		memcpy( cluster_buffer, currentPos, writeSize );
		if ( writeSize < CLUSTER_SIZE ) {
			memset( cluster_buffer + writeSize, 0x00, CLUSTER_SIZE-writeSize );
		}
		write_cluster( nextCluster );
		
		thisCluster = nextCluster;
		leftToWrite -= writeSize;
		
	}
	
	fat[thisCluster] = 0xFFFFFFFF;
	
}


// Shorthand macros for dereferencing values at an offset in the cluster buffer
#define	CGET8(x)	(*(cluster_buffer+x))
#define	CGET16(x)	(*((uint16_t*)(cluster_buffer+x)))
#define	CGET32(x)	(*((uint32_t*)(cluster_buffer+x)))


/*
** PUBLIC FUNCTIONS
*/

void _fs_init( void ) {
	
	if ( fs_initialized == 0 ) {
		
		c_printf("initializing ofs...\n");
		clear_buffer();
		
		int i;
		c_printf("about to read boot cluster\n");
		// Read boot sector
		status = read_sector( BOOT_CLUSTER, cluster_buffer );
		if ( status ) {
			c_printf("error reading boot cluster\n");
			return;
		}
		c_printf("disk read\n");
		
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
			status = load_fat();
			if (status) {
				//error
				c_printf("error loading fat\n");
				return;
			}
			
			// Load dt
			status = load_dt();
			if (status) {
				//error
				c_printf("error loading dt\n");
				return;
			}
			
			c_printf("ofs successfully loaded\n");
			
		} else {
			// Initialize the boot sector on this device
			c_printf("ofs not found, formatting device\n");
			current_dt = 1;
			current_fat = 2;
			
			CGET32(0) = CLUSTER_SIZE;
			CGET32(4) = FS_SIZE;
			CGET32(8) = current_dt;
			CGET32(12) = current_fat;
			CGET32(16) = SIGNATURE;
			
			// write it to the drive
			
			status = write_cluster( BOOT_CLUSTER );
			if ( status ) {
				// handle error
				c_printf("error writing boot cluster\n");
				return;
			}
			
			clear_buffer();
			
			// Initialize DT
			
			for ( i = 0; i < DIR_TABLE_SIZE; ++i ) {
				dir_table[i].name[0] = 0x00;
			}
			status = update_dt();
			if (status) {
				// error updaing dt
				return;
			}
			
			// Initialize the FAT
			memset( fat, 0x00, CLUSTER_COUNT*sizeof(uint32_t) );
			memset( fat, 0xFF, 3*sizeof(uint32_t) );
			
			// Update FAT
			status = update_fat();
			if (status) {
				// handle error
				return;
			}
			
			c_printf("ofs successfully created\n");
			
		}
		
		for ( i = 0; i < MAXFILES; ++i ) {
			fd[i].name[0]=0x00;
		}
		fs_initialized = 1;
	}
}

file_t * fcreat( char * name ) {
	int dti;
	int newfd = get_free_fd();
	if ( newfd != -1 ) {
		dti = find_in_dt( name );
		if ( dti == -1 ) {
			// File does not exist, lets try:
			dti = get_free_dti();
			if ( dti != -1 ) {
				// Create a DT entry
				strncpy( dir_table[dti].name, name, 112 );
				dir_table[dti].size = 0;
				dir_table[dti].type = 0xFFFFFFFF;
				dir_table[dti].time = _system_time;
				
				uint32_t fi = get_free_fat();
				dir_table[dti].index = fi;
				fat[fi] = 0xFFFFFFFF;
				
				update_dt();
				update_fat();
				
				return fopen( name );
			}
		}
	}
	
	return 0;
}

file_t * fopen( char * name ) {
	int newfd = get_free_fd();
	if ( newfd != -1 ) {
		int dti = find_in_dt( name );
		if ( dti != -1 ) {
			file_t *result = &fd[newfd];
			strncpy( result->name, dir_table[dti].name, 112 );
			result->cluster = dir_table[dti].index;
			result->id = newfd;
			result->size = dir_table[dti].size;
			return result;
		}
	}
	
	return 0;
}

int fclose( file_t * file ) {
	
	if ( file != 0 ) {
	
		// Update DT info for this file 
		int dti = find_in_dt( file->name );
		dir_table[dti].size = file->size;
		dir_table[dti].time = _system_time;
		
		// Clear file struct data
		file->name[0] = 0x00;
		file->cluster = 0;
		file->size = 0;
		file = 0;
		
		update_dt();
		update_fat();
	}
	
	return 0;
}

size_t fread( void * buffer, size_t size, size_t count, file_t * file ) {
	
	if ( file != 0 ) {
		read_data( file->cluster, (uint8_t**)&buffer, size*count );
	}
	
	return (size*count);
}

size_t fwrite( const void * buffer, size_t size, size_t count, file_t * file ) {
	
	if ( file != 0 ) {
		write_data( file->cluster, (uint8_t*)buffer, size*count );
		file->size = (size*count);
	}
	
	return (size*count);
}

// Print n fat and n dt listings for testing purposes, won't be available
void print_fat( int n ) {
	int i;
	n = CLUSTER_COUNT < n ? CLUSTER_COUNT : n;
	
	c_printf("File allocation table:\n");
	for (i = 0; i < n; ++i) {
		c_printf("fat[%d]=%08x\n", i, fat[i]);
	}
}

void print_dt( int n ) {
	int i;
	n = DIR_TABLE_SIZE < n ? DIR_TABLE_SIZE : n;
	
	c_printf("Directory Table:\n");
	for (i = 0; i < n; ++i) {
		if (dir_table[i].name[0] != 0x00 &&
			dir_table[i].name[0] != 0xFF) {
			c_printf("Entry #%d\n", i);
			c_printf("\tName: %s\n", dir_table[i].name);
			c_printf("\tIndex: %d\n", dir_table[i].index);
			c_printf("\tSize: %d\n", dir_table[i].size);
			c_printf("\tType: %s\n", dir_table[i].type == 0xFFFFFFFF 
				? "File" : "Other");
			c_printf("\tTime: %d\n", dir_table[i].time);
		}
	}
}

void df( void ) {
	c_printf("ofs Statistics\n");
	c_printf("Cluster size (bytes): %d\n", CLUSTER_SIZE);
	c_printf("Total Clusters: %d\n", CLUSTER_COUNT);
	int used_clusters = 0;
	int i;
	
	for (i = 0; i < CLUSTER_COUNT; ++i) {
		if (fat[i] != 0x00000000) {
			++used_clusters;
		}
	}
	
	c_printf("Used Clusters: %d\n", used_clusters);
	c_printf("Available Clusters: %d\n", CLUSTER_COUNT-used_clusters);
	float percent = ((float)CLUSTER_COUNT-(float)used_clusters)
		*((float)100/(float)CLUSTER_COUNT);
	c_printf("Percentage Free: %f\n", percent);

}

void stat( char * filename ) {
	int dti = find_in_dt( filename );
	
	if ( dti != -1 ) {
		c_printf(" File: %s\n", filename);
		c_printf("    Size: %d\n", dir_table[dti].size );
		c_printf("    Index: %d\n", dir_table[dti].index );
		c_printf("    Time: %08x\n", dir_table[dti].time );
		c_printf("    Type: %s\n", dir_table[dti].type == 0xFFFFFFFF 
				? "File" : "Other");
	}
	
}

void ls( void ) {
	int i;
	
	for (i = 0; i < DIR_TABLE_SIZE; ++i) {
		if (dir_table[i].name[0] != 0x00 &&
			dir_table[i].name[0] != 0xFF) {
			c_printf("%s\n", dir_table[i].name);
		}
	}
}

void rm( char * filename ) {
	int dti = find_in_dt( filename );
	
	if ( dti != -1 ) {
		c_printf("removing: %s\n", filename);
		dir_table[dti].name[0] = 0x00;
		clear_fat_chain( dir_table[dti].index );
		
		update_dt();
		update_fat();
	}
}
