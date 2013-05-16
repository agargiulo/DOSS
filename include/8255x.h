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
#include <pci.h>

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
// Can either read 1 byte or two. If you read one, you also
// will want to read the next byte separately
#define E_CSR_SCB_STAT_WORD     0x00    /* 2 Bytes */
// STAT_ACK is the upper byte of the STAT_WORD
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
/* Command completed */
#define SCB_STAT_CX             0x80
/* Frame received */
#define SCB_STAT_FR             0x40
/* CU went from active to idle */
#define SCB_STAT_CNA            0x20
/* RU went from active to idle or suspended */
#define SCB_STAT_RNR            0x10
/* MDI read or write cycle completed */
#define SCB_STAT_MDI            0x08
/* Software interrupt */
#define SCB_STAT_SWI            0x04
// 0x02 is reserved
/* Flow control paused */
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

/*
 * Action Command defines
 */
/* Last block in the CBL or RFA */
#define ACT_CMD_EL              0x8000
/* Suspend the CU/RU after cmd or frame */
#define ACT_CMD_S               0x4000
/* Generate an interrupt on completion */
#define ACT_CMD_I               0x2000
#define ACT_CMD_NOP             0x0000
// IA = Individual Address
#define ACT_CMD_IA_SETUP        0x0001
#define ACT_CMD_CONF            0x0002
// MA = Multicast Address
#define ACT_CMD_MA_SETUP        0x0003
#define ACT_CMD_TRANS           0x0004
#define ACT_CMD_LOAD_MCODE      0x0005
#define ACT_CMD_DUMP            0x0006
#define ACT_CMD_DIAGNOSE        0x0007

#define ACT_CMD_STAT_C          0x8000
#define ACT_CMD_STAT_OK         0x2000


#ifndef __SP2_ASM__

/*
 * Start of C-only definitions
 */

/*
 * Types
 */
/* 6 bytes in a MAC Address */
#define MAC_ADDR_LEN            6
/* Max bytes in an ethernet frame data area*/
#define FRAME_DAT_LEN           1496
/* Bytes in an ethernet header */
#define FRAME_HEAD_LEN          14
/* because I had to pick something */
#define E100_NUM_RX_BUFS        5

/*
 * A MAC address
 * Composed of 6 bytes
 */
typedef uint8_t mac_addr_t[MAC_ADDR_LEN];

/*
 * Ethernet frames and all that jazz
 * These are packed to ensure correct spacing of the data
 */
typedef struct ether_header
{
	mac_addr_t dest; // Where is this packet headed?
	mac_addr_t src; // Where did this packet come from?
	uint16_t protocol; // How should we interpret the info?
} __attribute__((__packed__)) ether_header_t;

typedef struct ether_frame
{
	ether_header_t eth_head; // See above
	uint8_t data[FRAME_DAT_LEN]; // Yum data =]
} __attribute__((__packed__)) ether_frame_t;


/*
 * The default structure that the card will use for commands sent to it
 * or for frames received by it
 */
typedef struct e100_cmd_header
{
	uint16_t stat;
	uint16_t cmd;
	uint32_t link_offset;
}  __attribute__((__packed__))e100_cmd_header_t;

/*
 * Used by the dump command
 * Dumps a ton of information into a large buffer
 */
typedef struct e100_cmd_dump
{
	e100_cmd_header_t header;
	uint32_t buff_addr;
	uint8_t buffer[596];
} __attribute__((__packed__)) e100_cmd_dump_t;

/*
 * stores information about data received
 */
typedef struct e100_rx_buf
{
	e100_cmd_header_t header; // See above

	uint32_t reserved;
	uint16_t true_count; // bytes actually written to the data buffer
	uint16_t size; // size of data buffer
	uint8_t frame[FRAME_DAT_LEN + FRAME_HEAD_LEN]; // data buffer
} __attribute__((__packed__)) e100_rx_buf_t;

/*
 * stores data to be sent out
 */
typedef struct e100_tx_buf
{
	e100_cmd_header_t header; // see above

	uint32_t tbda_addr; // addr of the transmit buffer descripter (TBD) array
	uint16_t tx_cb_byte_count; // how many bytes will be tx'd from the tx command block
	uint8_t tx_thresh; // how many bytes are present before tx happens
	uint8_t tbd_number; // num of tx buffers in the TBD
} __attribute__((__packed__)) e100_tx_buf_t;

/*
 * All of the information about the network card
 * that I care to save under a single roof
 */
typedef struct e100_device
{
	device_t *pci; // Basic PCI information. More can be set that I actually use
	uint32_t CSR_BAR; // Control/Status Register IO Mapped BAR
	mac_addr_t hw_addr; // Local MAC address

	/*
	 * For the Receive Frame Area
	 */
	e100_rx_buf_t *rx_buf_base;
	e100_rx_buf_t *rx_buf_ptr;
	e100_rx_buf_t *rx_buf_end;

	// Transmit things
	e100_tx_buf_t *tx_buf_base;

	// Did our command finish yet?
	// Check this and find out
	uint8_t CU_finished;

	// How many frames have we received so far?
	uint32_t rx_count;

} e100_device_t;

/*
 * Globals
 */
// The actual network device is controlled using info found in this struct
extern e100_device_t eth0;

/*
 * Prototypes
 */
/*
 * Initialize the network card and other related items
 */
void _net_init(void);

/*
 * Because interrupts need to be turned on for part of the
 * initialization to happen, this second init function gets
 * called from within the ISR after the first interrupt.
 */
void _net_complete_init(void);

/*
 * set of the RX Frame Area (RFA) for the RU
 */
void _net_init_rx_frame_area(void);

/*
 * Initialize a rx buffer
 */
void _net_init_rxb(e100_rx_buf_t *rx_buffer);

/*
 * use this to send a frame
 */
void e100_tx (uint8_t *tx_data, uint16_t data_size);

/*
 * Print out various debugging information
 */
void net_pci_dump(void);
void net_CSR_dump(void);

/*
 *  After every command we need to wait for the command register to
 *  read 0 to ensure that the command was received by the device
 */
void nic_wait(void);

/*
 * These are to make life easier.
 * They could just as easily be macros, but I hate macros
 */
uint8_t eth_pci_readb (uint8_t reg);
uint16_t eth_pci_read (uint8_t reg);
uint32_t eth_pci_readl (uint8_t reg);


/*
 * Take host integers and convert them to network bit order
 * and vice versa
 * Little Endianness <-> Big Enidanness
 */
uint32_t htonl(uint32_t hlong);
uint16_t htons(uint16_t hshort);
uint32_t ntohl(uint32_t nlong);
uint16_t ntohs(uint16_t nshort);

#endif // __SP2_ASM__

#endif
