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
/*
 * Map mac_addr_t to a hostname (char *)
 * used for host lookup functions
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

/*
 * translate hostname to mac address
 */
mac_addr_t *get_mac_address(char * hostname);

/*
 * looks up a hostname in the host_list
 * @return 1 if hostname is in the list,
 *         0 otherwise
 */
uint8_t is_known_host(mac_addr_t *hw_addr);

/*
 * prints out a mac address in the following format:
 * 01:23:45:67:89:AB
 * without any trailing or leading characters (no \n)
 */
void print_hw_addr(mac_addr_t *hw_addr);

#endif

#endif
