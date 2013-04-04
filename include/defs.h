/*
** SCCS ID:	@(#)defs.h	1.1	4/2/13
**
** File:	defs.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	General system definitions
*/

#ifndef _DEFS_H_
#define _DEFS_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

// Null pointer (defined the correct way)

#define	NULL		0

// ustatus_t status values

#define U_SUCCESS		0
#define	U_FAILURE		1
#define	U_NOT_FOUND		2
#define	U_ALREADY_DEAD		3

#define	N_USTATUS		4

// estatus_t status values

#define EXIT_SUCCESS		0
#define	EXIT_FAILURE		1
#define	EXIT_DEFAULT		2
#define	EXIT_BAD_SYSCALL	3

#define	N_ESTATUS		4

// Maximum number of simultaneous user processes

#define	N_PROCESSES	25

#ifdef __SP2_KERNEL__

// status_t status values

#define	SUCCESS		0
#define	FAILURE		1
#define	BAD_PARAM	2
#define	EMPTY_QUEUE	3
#define	NOT_EMPTY_QUEUE	4
#define	NOT_FOUND	5
#define	NO_QNODES	6

#define	N_STATUS	7

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

#include <types.h>

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

#endif

#endif

#endif
