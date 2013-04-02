/*
** SCCS ID:	@(#)ulibc.c	1.1	4/2/13
**
** File:	ulibc.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	C implementations of user-level library functions
*/

#include "common.h"

/*
** PRIVATE DEFINITIONS
*/

// ustatus_t value check

#define	ustatus(n) \
	((n) >= N_USTATUS ? "bad status value" : ustatus_strings[(n)])

// estatus_t value check

#define	estatus(n) \
	((n) >= N_ESTATUS ? "bad status value" : estatus_strings[(n)])

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

// ustatus_t strings

const char *ustatus_strings[] = {
	"success",		// U_SUCCESS
	"failure",		// U_FAILURE
	"not found",		// U_NOT_FOUND
	"already dead"		// U_ALREADY_DEAD
};

// estatus_t strings

const char *estatus_strings[] = {
	"success",		// EXIT_SUCCESS
	"failure",		// EXIT_FAILURE
	"default",		// EXIT_DEFAULT
	"bad syscall"		// EXIT_BAD_SYSCALL
};

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*      
** prt_ustat - print a ustatus_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/
        
void prt_ustat( char *msg, ustatus_t stat ) {

	if( msg == NULL ) {
		return;
	}

	c_printf( msg, ustatus(stat) );

	if( stat >= N_USTATUS ) {
		c_printf( "bad code: %d", stat );
									        	}
										}

/*      
** prt_estat - print an estatus_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/
        
void prt_estat( char *msg, estatus_t stat ) {

	if( msg == NULL ) {
		return;
	}

	c_printf( msg, estatus(stat) );

	if( stat >= N_ESTATUS ) {
		c_printf( "bad code: %d", stat );
									        	}
										}
