/*
 * File:	8255x.h
 *
 * Author:	Anthony Gargiulo
 *
 * Description: Header file to define all of the macros needed by the 8255x code
 */

#ifndef _8255X_H_
#define _8255X_H_

#include <common.h>

/*
 * General (C and/or assembly) definitions
 */
#define NET_DEVICE_ID           0x1229  /* Intel Pro 100 82559c (rev 8) */

/* http://forum.osdev.org/viewtopic.php?f=1&t=26609 */
#define PCI_BAR_IOMAP_MASK      0xFFFFFFFC
/*
 * PCI Configuration Space
 */
#define P_ETH_VEND_ID           0x00    /* 2 Bytes */
#define P_ETH_DEV_ID            0x02    /* 2 Bytes */
#define P_ETH_CMD_REG           0x04    /* 2 Bytes */
#define P_ETH_STATUS_REG        0x06    /* 2 Bytes */
#define P_ETH_REV_ID            0x08    /* 1 Byte  */
#define P_ETH_CLASS_CODE        0x09    /* 3 Bytes */
#define P_ETH_CACHE_LINE_SIZE   0x0C    /* 1 Bytes */
#define P_ETH_LAT_TIMER         0x0D    /* 1 Byte  */
#define P_ETH_HEADER_TYPE       0x0E    /* 1 Byte  */
#define P_ETH_BIST              0x0F    /* 1 Byte  */
#define P_ETH_CSR_IO_MAP_BAR    0X14    /* 4 Bytes */
#define P_ETH_FLASH_MEM_MAP_BAR 0x18    /* 4 Bytes */
#define P_ETH_SUBSYS_VEND_ID    0x2C    /* 2 Bytes */
#define P_ETH_SUBSYS_ID         0x2E    /* 2 Bytes */
#define P_ETH_EX_ROM_BAR        0x30    /* 4 Bytes */
#define P_ETH_CAP_PTR           0x34    /* 1 Byte  */
#define P_ETH_INT_LINE          0x3C    /* 1 Byte  */
#define P_ETH_INT_PIN           0x3D    /* 1 Byte  */
#define P_ETH_MIN_GRANT         0x3E    /* 1 Byte  */
#define P_ETH_MAX_LATENCY       0x3F    /* 1 Byte  */
#define P_ETH_CAP_ID            0xDC    /* 1 Byte  */
#define P_ETH_NXT_ITM_PTR       0xDD    /* 1 Byte  */
#define P_ETH_POW_MAN_CAP       0xDE    /* 2 Bytes */
#define P_ETH_POW_MAN_CSR       0xE0    /* 2 Bytes */
#define P_ETH_DATA              0xE2    /* 1 Byte  */

/*
 * Control / Status Register Offsets
 */
#define E_CSR_SCB_STAT_WORD     0x00    /* 2 Bytes */
#define E_CSR_SCB_STAT_ACK      0x01    /* 1 Byte  */
#define E_CSR_SCB_COM_WORD      0x02    /* 2 Bytes */
#define E_CSR_SCB_GEN_PTR       0x04    /* 4 Bytes */
#define E_CSR_PORT              0x08    /* 4 Bytes */
#define E_CSR_EEPROM_CON_REG    0x0E    /* 2 Bytes */
#define E_CSR_MDI_CON_REG       0x10    /* 4 Bytes */
#define E_CSR_RX_DMA_BYTES      0x14    /* 4 Bytes */
#define E_CSR_FLO_CON_REG       0x1A    /* 2 Bytes */
#define E_CSR_PMDR              0x1B    /* 1 Byte  */
#define E_CSR_GEN_CON           0x1C    /* 1 Byte  */
#define E_CSR_GEN_STAT          0x1D    /* 1 Byte  */
#define E_CSR_FUNC_EV_REG       0x30    /* 4 Bytes */
#define E_CSR_FUNC_EV_MSK_REG   0x34    /* 4 Bytes */
#define E_CSR_FUNC_PRE_STAT_REG 0x38    /* 4 Bytes */
#define E_CSR_FORCE_EV_REG      0x3C    /* 4 Bytes */

/*
 * Control Unit Commands
 */
#define SCB_CCMD_NOP            0x00
#define SCB_CCMD_CU_START       0x10
#define SCB_CCMD_CU_RESUME      0X20
// Load Dump Counters Address
#define SCB_CCMD_LDCA           0x40
// Dump Statistical Counters
#define SCB_CCMD_DSC            0x50
#define SCB_CCMD_LOAD_CU_BASE   0x60
// Dump and Reset Statistical Conters
#define SCB_CCMD_DRSC           0x70
#define SCB_CCMD_CU_S_RESUME    0xA0

/*
 * Receive Unit Commands
 */
#define SCB_RCMD_NOP            0x00
#define SCB_RCMD_RU_START       0x01
#define SCB_RCMD_RU_RESUME      0x02
// Receive DMA Redirect
#define SCB_RCMD_RECV_DMA_REDIR 0x03
#define SCB_RCMD_RU_ABORT       0x04
// Load Header Data Size (HDS)
#define SCB_RCMD_LOAD_HDS       0x05
#define SCB_RCMD_LOAD_RU_BASE   0x06


/*
 * STAT/ACK Defines
 */
#define SCB_STAT_CX             0x80
#define SCB_STAT_FR             0x40
#define SCB_STAT_CNA            0x20
#define SCB_STAT_RNR            0x10
#define SCB_STAT_MDI            0x08
#define SCB_STAT_SWI            0x04
// 0x02 is reserved
#define SCB_STAT_FCP            0x01
/*
 * CUS/RUS Values
 */
#define SCB_CUS_IDLE            0x00
#define SCB_CUS_SUSP            0x40
#define SCB_CUS_LPQ_ACT         0x80
#define SCB_CUS_HQP_ACT         0xC0

#define SCB_RUS_IDLE            0x00
#define SCB_RUS_SUSP            0x04
#define SCB_RUS_NO_RES          0x08
#define SCB_RUS_READY           0x10


#ifndef __SP2_ASM__

/*
 * Start of C-only definitions
 */

/*
 * Types
 */
#define MAC_ADDR_LEN            6       /* 6 bytes in a MAC Address */
typedef uint8_t[MAC_ADDR_LEN] mac_addr_t;

typedef struct e100_cmd_header
{
	uint16_t stat;
	uint16_t cmd;
	uint32_t link_offset;
} e100_cmd_header_t;

typedef struct e100_cmd_dump
{
	e100_cmd_header_t header;
	uint32_t buff_addr;
	uint8_t buffer[596];
} e100_cmd_dump_t;

typedef struct e100_device
{
	device_t *pci;
	uint32_t CSR_BAR;
	mac_addr_t hw_addr;
	uint8_t CU_finished;
} e100_device_t;

/*
 * Globals
 */
extern uint32_t CSR_BAR;

/*
 * Prototypes
 */
void _net_init(void);
void net_pci_dump(void);
void net_CSR_dump(void);
void nic_wait(void);
uint8_t eth_pci_readb (uint8_t reg);
uint16_t eth_pci_read (uint8_t reg);
uint32_t eth_pci_readl (uint8_t reg);

#endif // __SP2_ASM__

#endif
