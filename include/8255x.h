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
#define P_ETH_CSR_MEM_MAP_BAR   0X10    /* 4 Byte  */
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

#ifndef __SP2_ASM__

/*
 * Start of C-only definitions
 */

/*
 * Types
 */

/*
 * Globals
 */

/*
 * Prototypes
 */
void _net_init(void);
void net_pci_dump(void);

#endif

#endif
