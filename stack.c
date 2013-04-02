/*
** SCCS ID:	@(#)stack.c	1.1	4/2/13
**
** File:	stack.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Stack module implementation
*/

#define	__SP2_KERNEL__

#include "common.h"

#include "stack.h"
#include "queue.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

static stack_t _stacks[ N_STACKS ];	// user process stacks
queue_t _free_stacks;			// list of available stacks

/*
** PUBLIC GLOBAL VARIABLES
*/

stack_t _system_stack;			// stack for the OS
uint32_t *_system_esp;			// OS stack pointer

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _stack_init()
**
** initializes all stack-related data structures
*/

void _stack_init( void ) {
	int i;
	
	// clear the free stack queue

	_q_reset( &_free_stacks, NULL );
	
	// "free" all the stacks

	for( i = 0; i < N_STACKS; ++i ) {
		_stack_free( &_stacks[i] );
	}
	
	// report that we have finished

	c_puts( " stacks" );
}

/*
** _stack_alloc()
**
** allocate a stack structure
**
** returns a pointer to the stack, or NULL on failure
*/

stack_t *_stack_alloc( void ) {
	stack_t *stack;
	status_t stat;
	
	// pull the first available stack off the free queue

	stat = _q_remove( &_free_stacks, (void **) &stack );
	if( stat == BAD_PARAM ) {
		_kpanic( "_stack_alloc", "queue remove status %s", stat );
	} else if( stat == EMPTY_QUEUE ) {
		stack = NULL;
	}

	// return it to the caller (or NULL)

	return( stack );
}

/*
** _stack_free(stack)
**
** deallocate a stack, putting it into the list of available stacks
*/

void _stack_free( stack_t *stack ) {
	status_t stat;
	
	// sanity check:  avoid deallocating a NULL pointer
	if( stack == NULL ) {
		// this should probably be an error
		return;
	}

	// return the stack to the free list

	stat = _q_insert( &_free_stacks, (void *) stack, _key0 );
	if( stat != SUCCESS ) {
		_kpanic( "_stack_free", "free queue insert status %s", stat );
	}
}
