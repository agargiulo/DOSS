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
#include <pci.h>

/*
** PRIVATE DEFINITIONS
*/

#define SATA_CONTROLLER		1
#define DELAY400NS  { pio_inbyte( CB_ASTAT ); pio_inbyte( CB_ASTAT );  \
                      pio_inbyte( CB_ASTAT ); pio_inbyte( CB_ASTAT ); }
/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

#define MAX_CONTROLLERS 2

device_t disk_ctrl_tab[MAX_CONTROLLERS];
int ctrl_count;

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/


/*
** PUBLIC FUNCTIONS
*/

void _disk_init( void ) {
	
	int i;
	
	ctrl_count = 0;
	
	c_printf("searching for devices\n");
	
	// Find controllers in device tab
	for ( i = 0; i != device_count; ++i ) {
		if ( device_tab[i].class == MASS_STORAGE ) {
			// save it
			c_printf("found device: %d %d %d\n", device_tab[i].bus, device_tab[i].slot, device_tab[i].func);
			disk_ctrl_tab[ctrl_count++] = device_tab[i];
		}
	}
	
	
	
}

