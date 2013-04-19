/*
** SCCS ID:	@(#)klibc.c	1.1	4/2/13
**
** File:	klibc.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	C implementations of kernel library functions
*/

#define	__SP2_KERNEL__

#include <common.h>

#include <queue.h>
#include <stack.h>

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

// status value strings
//
// (must track status_t values in defs.h)

const char *_kstatus_strings[] = {
	"success",		// SUCCESS
	"failure",		// FAILURE
	"bad param",		// BAD_PARAM
	"empty queue",		// EMPTY_QUEUE
	"not empty queue",	// NOT_EMPTY_QUEUE
	"not found",		// NOT_FOUND
	"no qnodes"		// NO_QNODES
};

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*      
** _prt_stat - print a status_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/
        
void _prt_stat( char *msg, status_t stat ) {

	if( msg == NULL ) {
		return;
	}

	c_printf( msg, _kstatus(stat) );

	if( stat >= N_STATUS ) {
		c_printf( "bad code: %d", stat );
									        	}
										}

/*
** _put_char_or_code( ch )
**
** prints the character on the console, unless it is a non-printing
** character, in which case its hex code is printed
*/

void _put_char_or_code( int ch ) {

	if( ch >= ' ' && ch < 0x7f ) {
		c_putchar( ch );
	} else {
		c_printf( "\\x%02x", ch );
	}
}

/*
** _kmemclr - initialize all bytes of a block of memory to zero
**
** usage:  _kmemclr( buffer, length )
*/

void _kmemclr( register byte_t *buf, register uint32_t len ) {

	while( len-- ) {
		*buf++ = 0;
	}

}

/*
** _kmemcpy - copy a block from one place to another
**
** usage:  _kmemcpy( dest, src, length )
**
** may not correctly deal with overlapping buffers
*/

void _kmemcpy( register byte_t *dst, register byte_t *src, register uint32_t len ) {

	while( len-- ) {
		*dst++ = *src++;
	}

}


/*
** _kpanic - kernel-level panic routine
**
** usage:  _kpanic( mod, msg, code )
**
** Prefix routine for __panic() - can be expanded to do other things
** (e.g., printing a stack traceback)
**
** 'mod' argument is always printed; 'msg' argument is printed
** if it isn't NULL, followed by a newline
**
** 'msg' argument should contain a %s format code if the 'code'
** argument is to be printed as part of the panic message
**
** if 'code' is invalid, it will also be printed in integer form
*/

void _kpanic( char *mod, char *msg, status_t code ) {

	c_puts( "\n\n***** KERNEL PANIC *****\n\n" );
	c_printf( "Module: %s\n", mod );
	if( msg != NULL ) {
		c_printf( msg, _kstatus(code) );
		c_putchar( '\n' );
	}
	if( code >= N_STATUS ) {
		c_printf( "*** bad code %d\n", code );
	}

	//
	// This might be a good place to do a stack frame
	// traceback
	//

	// dump out all the queues

	_q_dump_all();

	__panic( "KERNEL PANIC" );

}
