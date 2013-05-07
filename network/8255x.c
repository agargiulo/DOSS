/*
 * File: network/8255x.c
 *
 * Author: Anthony Gargiulo (20123)
 *
 * Description: Driver for the Intel Pro 100 8255x (rev 8) card
 */

#define	__SP2_KERNEL__

#include <common.h>
#include <startup.h>
#include <support.h>
#include <pci.h>
#include <8255x.h>
#include <net_handler.h>

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE DATA TYPES
 */
device_t *eth0;

/*
 * PRIVATE GLOBAL VARIABLES
 */

/*
 * PUBLIC GLOBAL VARIABLES
 */
uint32_t CSR_BAR;

/*
 * PRIVATE FUNCTIONS
 */

/*
 * PUBLIC FUNCTIONS
 */

void _net_init(void)
{
	// Look through the device_tab for the network card
	// In this case, the 8255x (device id 0x1229)
	for (int i = 0; i < device_count; i++)
	{
		if (device_tab[i].device == 0x1229 || device_tab[i].device == 0x1019)
		{
			eth0 = &device_tab[i];
		}
	}

	if (eth0 == NULL)
	{
		c_puts("\nError locating a network device\n");
		return;
	}

	CSR_BAR = eth_pci_readl(P_ETH_CSR_IO_MAP_BAR);

	c_printf("\nCSR_BAR: 0x%08x\n", CSR_BAR);

	uint8_t int_line = eth_pci_readb(P_ETH_INT_LINE);
	int_line += 0x20;
	c_printf("int_line: 0x%02x\n", int_line);
	__install_isr(int_line, _net_handler);


	c_puts(" network\n");
}

uint8_t eth_pci_readb (uint8_t reg)
{
	return pci_readb(eth0->bus, eth0->slot, eth0->func, reg);
}

uint16_t eth_pci_read (uint8_t reg)
{
	return pci_read(eth0->bus, eth0->slot, eth0->func, reg);
}

uint32_t eth_pci_readl (uint8_t reg)
{
	return pci_readl(eth0->bus, eth0->slot, eth0->func, reg);
}

void net_pci_dump(void)
{
	c_printf("Vendor ID--------0x%04x\n", eth0->vendor);
	c_printf("Device ID--------0x%04x\n", eth0->device);
	c_printf("Status-----------0x%04x\n", eth_pci_read(P_ETH_STATUS_REG));
	c_printf("Command----------0x%04x\n", eth_pci_read(P_ETH_CMD_REG));
	c_printf("Class------------0x%02x0000\n", eth0->class);
	c_printf("Rev ID-----------0x%02x\n", eth_pci_readb(P_ETH_REV_ID));
	c_printf("BIST-------------0x%02x\n", eth_pci_readb(P_ETH_BIST));
	c_printf("Head Type--------0x%02x\n", eth_pci_readb(P_ETH_HEADER_TYPE));
	c_printf("Lat Timer--------0x%02x\n", eth_pci_readb(P_ETH_LAT_TIMER));
	c_printf("Cache Line Size--0x%02x\n", eth_pci_readb(P_ETH_CACHE_LINE_SIZE));
	c_printf("CSR IMap BAR-----0x%08x\n", CSR_BAR);
	c_printf("Subsys ID--------0x%04x\n", eth_pci_read(P_ETH_SUBSYS_ID));
	c_printf("Subsys Vend ID---0x%04x\n", eth_pci_read(P_ETH_SUBSYS_VEND_ID));
	c_printf("EEPROM BAR-------0x%08x\n", eth_pci_readl(P_ETH_EX_ROM_BAR));
	c_printf("Cap_Ptr----------0x%02x\n", eth_pci_readb(P_ETH_CAP_PTR));
	c_printf("Max Latency------0x%02x\n", eth_pci_readb(P_ETH_MAX_LATENCY));
#ifdef ISR_DEBUGGING_CODE
	c_puts("---MORE---");
	c_getchar();
	c_puts("\n");
#endif
	c_printf("Min Grant--------0x%02x\n", eth_pci_readb(P_ETH_MIN_GRANT));
	c_printf("Inter Pin--------0x%02x\n", eth_pci_readb(P_ETH_INT_PIN));
	c_printf("Inter Line-------0x%02x\n", eth_pci_readb(P_ETH_INT_LINE));
	c_printf("Pow Man Cap------0x%04x\n", eth_pci_read(P_ETH_POW_MAN_CAP));
	c_printf("Next Item Prt----0x%02x\n", eth_pci_readb(P_ETH_NXT_ITM_PTR));
	c_printf("Cap ID-----------0x%02x\n", eth_pci_readb(P_ETH_CAP_ID));
	c_printf("Data-------------0x%02x\n", eth_pci_readb(P_ETH_DATA));
	c_printf("Pow Man CSR------0x%04x\n", eth_pci_read(P_ETH_POW_MAN_CSR));
}

void net_CSR_dump( void )
{
	c_printf("SCB Stat-------------0x%04x\n", __inw(CSR_BAR + E_CSR_SCB_STAT_WORD));
	c_printf("SCB Comm-------------0x%04x\n", __inw(CSR_BAR + E_CSR_SCB_COM_WORD));
	c_printf("SCB Gen Ptr----------0x%08x\n", __inl(CSR_BAR + E_CSR_SCB_GEN_PTR));
	c_printf("PORT-----------------0x%08x\n", __inl(CSR_BAR + E_CSR_PORT));
	c_printf("EEPROM Con Reg-------0x%04x\n", __inw(CSR_BAR + E_CSR_EEPROM_CON_REG));
	c_printf("MDI Con Reg----------0x%08x\n", __inl(CSR_BAR + E_CSR_MDI_CON_REG));
	c_printf("RX DMA Bytes---------0x%08x\n", __inl(CSR_BAR + E_CSR_RX_DMA_BYTES));
	c_printf("Flow Control---------0x%04x\n", __inw(CSR_BAR + E_CSR_FLO_CON_REG));
	c_printf("PMDR-----------------0x%02x\n", __inb(CSR_BAR + E_CSR_PMDR));
	c_printf("Gen Control----------0x%02x\n", __inb(CSR_BAR + E_CSR_GEN_CON));
	c_printf("Gen Status-----------0x%02x\n", __inb(CSR_BAR + E_CSR_GEN_STAT));
	c_printf("Func Event Reg-------0x%08x\n", __inl(CSR_BAR + E_CSR_FUNC_EV_REG));
	c_printf("Func Eve Mask Reg----0x%08x\n", __inl(CSR_BAR + E_CSR_FUNC_EV_MSK_REG));
	c_printf("Func Pres State Reg--0x%08x\n", __inl(CSR_BAR + E_CSR_FUNC_PRE_STAT_REG));
	c_printf("Force Event Reg------0x%08x\n", __inl(CSR_BAR + E_CSR_FORCE_EV_REG));
	c_puts("---MORE---");
	c_getchar();
	c_putchar('\n');
}
