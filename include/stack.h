/*
** SCCS ID:	@(#)stack.h	1.1	4/2/13
**
** File:	stack.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Stack module declarations
*/

#ifndef _STACK_H_
#define _STACK_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

// stack size (in longwords)

#define	STACK_SIZE	1024

// number of user stacks

#define	N_STACKS	N_PROCESSES

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

// user stack

typedef uint32_t	stack_t[ STACK_SIZE ];

/*
** Globals
*/

extern stack_t _system_stack;		// stack for the OS
extern uint32_t *_system_esp;		// OS stack pointer

/*
** Prototypes
*/

/*
** _stack_init()
**
** initializes all stack-related data structures
*/

void _stack_init( void );

/*
** _stack_alloc()
**
** allocate a stack structure
**
** returns a pointer to the stack, or NULL on failure
*/

stack_t *_stack_alloc( void );

/*
** _stack_free(stack)
**
** deallocate a stack, putting it into the list of available stacks
*/

void _stack_free( stack_t *stack );

#endif

#endif
