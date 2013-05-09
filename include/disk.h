/*
**
** File:	disk.h
**
** Author:	4003-506 class of 20123
**
** Contributor:	Owen Royall-Kahin
**
** Description:	disk module header file
*/

#ifndef _DISK_H_
#define _DISK_H_

#define __SP2_KERNEL__

#include <common.h>
#include <types.h>

/*
** General (C and/or assembly) definitions
*/


/*
** Start of C-only definitions
*/

// ATA Offsets

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Errors

#define CB_ER_ICRC 0x80    // ATA Ultra DMA bad CRC
#define CB_ER_BBK  0x80    // ATA bad block
#define CB_ER_UNC  0x40    // ATA uncorrected error
#define CB_ER_MC   0x20    // ATA media change
#define CB_ER_IDNF 0x10    // ATA id not found
#define CB_ER_MCR  0x08    // ATA media change request
#define CB_ER_ABRT 0x04    // ATA command aborted
#define CB_ER_NTK0 0x02    // ATA track 0 not found
#define CB_ER_NDAM 0x01    // ATA address mark not found

// Statuses
#define	ATA_SR_BSY	0x80
#define	ATA_SR_DRDY	0x40
#define	ATA_SR_DF	0x20
#define	ATA_SR_WFT	0x20
#define	ATA_SR_DSC	0x10
#define	ATA_SR_SERV	0x10
#define	ATA_SR_DRQ	0x08
#define	ATA_SR_CORR	0x04
#define	ATA_SR_IDX	0x02
#define	ATA_SR_ERR	0x01


// Identification space
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

// ATA Commands
#define CMD_CFA_ERASE_SECTORS            0xC0
#define CMD_CFA_REQUEST_EXT_ERR_CODE     0x03
#define CMD_CFA_TRANSLATE_SECTOR         0x87
#define CMD_CFA_WRITE_MULTIPLE_WO_ERASE  0xCD
#define CMD_CFA_WRITE_SECTORS_WO_ERASE   0x38
#define CMD_CHECK_POWER_MODE1            0xE5
#define CMD_CHECK_POWER_MODE2            0x98
#define CMD_DEVICE_RESET                 0x08
#define CMD_EXECUTE_DEVICE_DIAGNOSTIC    0x90
#define CMD_FLUSH_CACHE                  0xE7
#define CMD_FLUSH_CACHE_EXT              0xEA
#define CMD_FORMAT_TRACK                 0x50
#define CMD_IDENTIFY_DEVICE              0xEC
#define CMD_IDENTIFY_DEVICE_PACKET       0xA1
#define CMD_IDENTIFY_PACKET_DEVICE       0xA1
#define CMD_IDLE1                        0xE3
#define CMD_IDLE2                        0x97
#define CMD_IDLE_IMMEDIATE1              0xE1
#define CMD_IDLE_IMMEDIATE2              0x95
#define CMD_INITIALIZE_DRIVE_PARAMETERS  0x91
#define CMD_INITIALIZE_DEVICE_PARAMETERS 0x91
#define CMD_NOP                          0x00
#define CMD_PACKET                       0xA0
#define CMD_READ_BUFFER                  0xE4
#define CMD_READ_DMA                     0xC8
#define CMD_READ_DMA_EXT                 0x25
#define CMD_READ_DMA_QUEUED              0xC7
#define CMD_READ_DMA_QUEUED_EXT          0x26
#define CMD_READ_MULTIPLE                0xC4
#define CMD_READ_MULTIPLE_EXT            0x29
#define CMD_READ_SECTORS                 0x20
#define CMD_READ_SECTORS_EXT             0x24
#define CMD_READ_VERIFY_SECTORS          0x40
#define CMD_READ_VERIFY_SECTORS_EXT      0x42
#define CMD_RECALIBRATE                  0x10
#define CMD_SEEK                         0x70
#define CMD_SET_FEATURES                 0xEF
#define CMD_SET_MULTIPLE_MODE            0xC6
#define CMD_SLEEP1                       0xE6
#define CMD_SLEEP2                       0x99
#define CMD_SMART                        0xB0
#define CMD_STANDBY1                     0xE2
#define CMD_STANDBY2                     0x96
#define CMD_STANDBY_IMMEDIATE1           0xE0
#define CMD_STANDBY_IMMEDIATE2           0x94
#define CMD_WRITE_BUFFER                 0xE8
#define CMD_WRITE_DMA                    0xCA
#define CMD_WRITE_DMA_EXT                0x35
#define CMD_WRITE_DMA_QUEUED             0xCC
#define CMD_WRITE_DMA_QUEUED_EXT         0x36
#define CMD_WRITE_MULTIPLE               0xC5
#define CMD_WRITE_MULTIPLE_EXT           0x39
#define CMD_WRITE_SECTORS                0x30
#define CMD_WRITE_SECTORS_EXT            0x34
#define CMD_WRITE_VERIFY                 0x3C

// BMIDE Registers

#define BM_COMMAND_REG    0            // offset to BM command reg
#define BM_CR_MASK_READ    0x00           // read from memory
#define BM_CR_MASK_WRITE   0x08           // write to memory
#define BM_CR_MASK_START   0x01           // start transfer
#define BM_CR_MASK_STOP    0x00           // stop transfer

#define BM_STATUS_REG     2            // offset to BM status reg
#define BM_SR_MASK_SIMPLEX 0x80           // simplex only
#define BM_SR_MASK_DRV1    0x40           // drive 1 can do dma
#define BM_SR_MASK_DRV0    0x20           // drive 0 can do dma
#define BM_SR_MASK_INT     0x04           // INTRQ signal asserted
#define BM_SR_MASK_ERR     0x02           // error
#define BM_SR_MASK_ACT     0x01           // active

#define BM_PRD_ADDR_LOW   4            // offset to BM prd addr reg low 16 bits
#define BM_PRD_ADDR_HIGH  6            // offset to BM prd addr reg high 16 bits

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

/**
 * Initialize the disk. Yup.
 */
void _disk_init( void );

/**
 * Prints the discovered devices
 */
void print_devices();

/**
 * Public function for reading data from the current disk
 */
status_t disk_read( uint32_t sector, uint8_t *buffer, size_t n );

/**
 * Public function for writing data to the current disk.
 */
status_t disk_write( uint32_t sector, const uint8_t *buffer, size_t n );


#endif
