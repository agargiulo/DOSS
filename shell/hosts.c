/*
** File: shell/hosts.c
**
** Author: Anthony Gargiulo
**
** Description: list various information about hosts on the local area network
*/

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

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

void run_hosts(int argc, char *argv[])
{
	if (argc < 2)
	{
		c_puts("Usage: hosts [-h|-a|-l|<hostname>]\n");
		return;
	}
	if (strcmp(argv[1], "-h") == 0)
	{
		c_puts("Usage: hosts [-h|-a|-l|hostname]\n");
		c_puts("\nThis command shows various bits of information about network related things\n\n");
		c_puts("-h : prints this message\n");
		c_puts("-a : prints information about every known host\n");
		c_puts("-l : prints information about this machine\n");
		c_puts("<hostname> : prints information about the given hostname\n");
		return;
	}
	else if (strcmp(argv[1], "-l") == 0)
	{
		c_printf("Hostname: %s.dsl.cs.rit.edu\nMAC address: ", get_hostname(&eth0.hw_addr));
		print_hw_addr(&eth0.hw_addr);
		c_putchar('\n');
	}
	else if (strcmp(argv[1], "-a") == 0)
	{
		c_puts("   Hostname    |    MAC Address    |  Seen?\n");
		for (int i = 0; i < num_known_hosts; i++)
		{
			c_printf(" %8s.dsl... | ", host_list[i].hostname);
			print_hw_addr(&host_list[i].hw_addr);
			c_printf(" | %s \n", ((memcmp(&eth0.hw_addr, &host_list[i].hw_addr, sizeof(mac_addr_t)) == 0) ? "yes" : "no" ));
		}
	}
	else // argv[1] is a hostname, print out info for that
	{
		char *hostname = argv[1];
		mac_addr_t *hw_addr = get_mac_address(hostname);
		c_puts("   Hostname    |    MAC Address    |  Seen?\n");
		c_printf(" %8s.dsl... | ", hostname);
		print_hw_addr(hw_addr);
		c_printf(" | %s \n", ((memcmp(&eth0.hw_addr, &hw_addr, sizeof(mac_addr_t)) == 0) ? "yes" : "no" ));
	}
}
