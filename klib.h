/*
** SCCS ID:	@(#)klib.h	1.1	4/2/13
**
** File:	klib.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Kernel support library declarations
*/

#ifndef _KLIB_H_
#define _KLIB_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

// pseudo-function:  return a string describing a status_t value

#define _kstatus(n) \
	((n) >= N_STATUS ? "bad status value" : _kstatus_strings[(n)])

/*
** Types
*/

/*
** Globals
*/

// status_t value strings

extern const char *_kstatus_strings[];

/*
** Prototypes
*/

/*      
** _prt_stat - print a status_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/

void _prt_stat( char *msg, status_t stat );

/*
** _get_ebp - return current contents of EBP at the time of the call
**
** Could be used, e.g., by _kpanic to print a traceback
*/

uint32_t _get_ebp( void );

/*
** _put_char_or_code( ch )
**
** prints the character on the console, unless it is a non-printing
** character, in which case its hex code is printed
*/

void _put_char_or_code( int ch );

/*
** _kmemclr - initialize all bytes of a block of memory to zero
**
** usage:  _kmemclr( buffer, length )
*/

void _kmemclr( register byte_t *buf, register uint32_t len );

/*
** _kmemcpy - copy a block from one place to another
**
** usage:  _kmemcpy( dest, src, length )
**
** may not correctly deal with overlapping buffers
*/

void _kmemcpy( register byte_t *dst, register byte_t *src, register uint32_t len );

/*
** _kpanic - kernel-level panic routine
**
** usage:  _kpanic( mod, msg, code )
**
** Prefix routine for __panic() - can be expanded to do other things
** (e.g., printing a stack traceback)
**
** 'msg' argument should contain a %s format code if the 'code'
** argument is to be printed as part of the panic message
*/

void _kpanic( char *mod, char *msg, status_t code );

#endif

#endif
