/*
** SCCS ID:	@(#)queue.h	1.1	4/2/13
**
** File:	queue.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Queue module declarations
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

// Pseudo-function:  is a queue empty?
#define _q_empty(qp)	(_q_status(qp) == EMPTY_QUEUE)

/*
** Types
**
** Our queues are generic, self-ordering queues.  The queue itself
** is a doubly-linked list of queue nodes, which in turn point to
** the information queued up in this position.  Each queue has an
** associated ordering routine which is used by the insertion 
** routine to order the queue elements.  This allows us to have
** different sorting criteria for different queues, but manage them
** with one set of functions.
*/

// queue node

typedef struct qnode {
	struct qnode *prev;	// "backward" link
	struct qnode *next;	// "forward" link
	void *data;		// what's queued up here
	key_t key;		// sorting key
} qnode_t;

// queue header

typedef struct queue {
	qnode_t *head;		// first entry
	qnode_t *tail;		// last entry
	int (*compare)( qnode_t *new, qnode_t *curr );	// ordering routine
} queue_t;

/*
** Globals
*/

// default system-wide "value 0" key

extern key_t _key0;

/*
** Prototypes
*/

/*
** _compare_t(qn1,qn2)
**
** compare two qnodes using the time_t key data member
*/

int _compare_t( register qnode_t *q1, register qnode_t *q2 );

/*
** _compare_u(qn1,qn2)
**
** compare two qnodes using the uint32_t key data member
*/

int _compare_u( register qnode_t *q1, register qnode_t *q2 );

/*
** _q_init()
**
** initialize the queue module
*/

void _q_init( void );

/*
** _q_status(que)
**
** evaluate the status of a queue
**
** returns the status
*/

status_t _q_status( queue_t *que );

/*
** _q_reset(que,fcn)
**
** reinitialize the specified queue 
*/

void _q_reset( queue_t *que, int (*compare)(qnode_t*,qnode_t*) );

/*
** _q_insert(que,data,key)
**
** insert the supplied data value into the queue, using the supplied
** key for ordering according to the queue's ordering rule
**
** returns the status of the insertion attemp
*/

status_t _q_insert( queue_t *que, void *data, key_t key );

/*
** _q_remove(que,data)
**
** remove the first element from the queue, returning the pointer to
** it through the second parameter
**
** returns the status of the removal attempt
*/

status_t _q_remove( queue_t *que, void **data );

/*
** _q_remove_specific(que,data,key)
**
** remove the element specified by the third parameter from the queue,
** returning the pointer to it through the second parameter
**
** returns the status of the removal attempt
*/

status_t _q_remove_specific( queue_t *que, void **data, key_t key );

/*
** _q_dump(which,queue)
**
** dump the contents of the specified queue to the console
*/

void _q_dump( char *which, queue_t *queue );

/*
** _q_dump_all()
**
** dump the contents of all the queues
*/

void _q_dump_all( void );

#endif

#endif
