/*
** File: network/net.c
**
** Author: Anthony Gargiulo
**
** Description: kernel api for network things
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <string.h>
#include <net.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

uint8_t num_known_hosts = 4;
host_map_t host_list[] = 
{   {{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xF8}, "gregson"},
	{{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xCD}, "adler"},
	{{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xBD}, "moriarty"},
	{{0x00, 0x90, 0x27, 0xBB, 0xDA, 0xD9}, "musgrave"} };

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

char *get_hostname(mac_addr_t hw_addr)
{
	for (int i = 0; i < num_known_hosts; i++)
	{
		if (memcmp(&hw_addr, &host_list[i].hw_addr, sizeof(mac_addr_t)) == 0)
		{
			return host_list[i].hostname;
		}
	}
	return NULL;
}

mac_addr_t *get_mac_address(char *hostname)
{
	for (int i = 0; i < num_known_hosts; i++)
	{
		if (strcmp(hostname, host_list[i].hostname) == 0)
		{
			return &host_list[i].hw_addr;
		}
	}
	return NULL;
}

uint8_t is_known_host(mac_addr_t hw_addr)
{
	return (get_hostname(hw_addr) != NULL ? 1 : 0);
}
