/*
** File: include/net.h
**
** Author: Anthony Gargiulo
**
** Description: Kernel level network API
*/

#ifndef _NET_H_
#define _NET_H_

#include <common.h>
#include <8255x.h>


/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/
typedef struct host_map
{
	mac_addr_t hw_addr;
	char *hostname;
} host_map_t;

extern host_map_t host_list[];
extern uint8_t num_known_hosts;
/*
** Globals
*/

/*
** Prototypes
*/

/*
 * translate mac address struct to hostname
 */
char *get_hostname(mac_addr_t *hw_addr);

mac_addr_t *get_mac_address(char * hostname);

uint8_t is_known_host(mac_addr_t *hw_addr);

void print_hw_addr(mac_addr_t *hw_addr);

#endif

#endif
