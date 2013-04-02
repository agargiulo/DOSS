/*
** SCCS ID:	@(#)system.h	1.1	4/2/13
**
** File:	system.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Miscellaneous OS support declarations
*/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "common.h"

#include "process.h"
#include "stack.h"

/*
** General (C and/or assembly) definitions
*/

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

/*
** _create_process(pcb)
**
** allocate and initialize a new process' data structures (PCB, stack)
**
** returns:
**      pointer to the new PCB
*/

pcb_t *_create_process( pcb_t *pcb );

/*
** _create_stack(stack)
**
** initialize a fresh user context in a process stack
**
** returns:
**      pointer to the context save area in the stack
*/

context_t *_create_stack( stack_t *stack );

/*
** _zombify(pcb)
**
** turn a process into a zombie, or give its status to
** a waiting parent
*/

void _zombify( pcb_t *pcb );

/*
** _init - system initialization routine
**
** Called by the startup code immediately before returning into the
** first user process.
*/

void _init( void );

#endif

#endif
