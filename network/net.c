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

uint8_t num_known_hosts = 5;
host_map_t host_list[] = 
{   {{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xF8}, "gregson"},
	{{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xCD}, "adler"},
	{{0x00, 0x90, 0x27, 0xBB, 0xD9, 0xBD}, "moriarty"},
	{{0x00, 0x90, 0x27, 0xBB, 0xDA, 0xD9}, "musgrave"},
	{{0x00, 0x90, 0x27, 0x90, 0x29, 0x9D}, "sholto"} };

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

char *get_hostname(mac_addr_t *hw_addr)
{
	for (int i = 0; i < num_known_hosts; i++)
	{
		if (memcmp(hw_addr, &host_list[i].hw_addr, sizeof(mac_addr_t)) == 0)
		{
			return host_list[i].hostname;
		}
	}
	return "unknown_host";
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
	static mac_addr_t nul_mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	return &nul_mac;
}

uint8_t is_known_host(mac_addr_t *hw_addr)
{
	return (get_hostname(hw_addr) != NULL ? 1 : 0);
}

void print_hw_addr(mac_addr_t *hw_addr)
{
	for (int i = 0; i < MAC_ADDR_LEN - 1; i++)
	{
		c_printf("%02x:", (*hw_addr)[i]);
	}
	c_printf("%02x", (*hw_addr)[MAC_ADDR_LEN - 1]);
}
