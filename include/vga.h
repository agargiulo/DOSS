/*
** SCCS ID:	%W%	%G%
**
** File:	vga.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Prototypes for vga mode initialization
*/

#ifndef _VGA_H_
#define _VGA_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

//Ports taken from http://files.osdev.org/mirors/geezer/osd/graphics/mode.c
#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define	VGA_GC_INDEX		0x3CE
#define	VGA_GC_DATA		0x3CF
#define	VGA_CRTC_INDEX		0x3D4      /* 0x3B4 */
#define	VGA_CRTC_DATA		0x3D5      /* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1+VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS)

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

void write_registers(unsigned char *regs);
void write_font( unsigned char *buf );

void set_plane(unsigned p);
static unsigned get_fb_seg( void );

void _video_setmode_13( void );
void _video_setmode_text( void );

#endif

#endif
