/*
** SCCS ID:	@(#)queue.c	1.1	4/2/13
**
** File:	queue.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Queue module implmentation
*/

#define	__SP2_KERNEL__

#include "common.h"

#include "queue.h"

#include "process.h"
#include "stack.h"

#include "syscall.h"
#include "sio.h"
#include "scheduler.h"

/*
** PRIVATE DEFINITIONS
*/

// number of qnodes to allocate
//
// need one per PCB, one per stack, and a few extra

#define	N_QNODES	(N_PCBS + N_STACKS + 5)

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

static qnode_t _qnodes[ N_QNODES ];	// the set of qnodes
static qnode_t *_free_qnodes;		// list of available qnodes

/*
** PUBLIC GLOBAL VARIABLES
*/

// default system-wide "value 0" key

key_t _key0;

/*
** PRIVATE FUNCTIONS
*/

/*
** _qnode_alloc()
**
** allocate a qnode, returning a pointer to it or NULL
*/

qnode_t *_qnode_alloc( void ) {
	qnode_t *new;
	
	// pull the first one off the free list

	new = _free_qnodes;
	if( new != NULL ) {
		_free_qnodes = new->next;
	}
	
	// return it to the caller

	return( new );
}

/*
** _qnode_free(qnode)
**
** deallocate a qnode, putting it into the list of available qnodes
*/

void _qnode_free( qnode_t *qnode ) {

	// sanity check:  don't deallocate a NULL pointer
	if( qnode == NULL ) {
		// this should probably be an error
		return;
	}

	// fastest method:  put this at the front of the list

	qnode->next = _free_qnodes;
	qnode->prev = NULL;
	_free_qnodes = qnode;
}


/*
** PUBLIC FUNCTIONS
*/

/*
** _compare_t(q1,q2)
**
** compare the key values from the supplied qnodes using the time_t member
**
** returns the status of the comparison:
**      < 0     q1 < q2
**      = 0     q1 == q2
**      > 0     q1 > q2
*/

int _compare_t( register qnode_t *q1, register qnode_t *q2 ) {

	// should really do a sanity check here, but what if
	// it fails?

	if( q1->key.t < q2->key.t ) return( -1 );
	else if( q1->key.t == q2->key.t ) return( 0 );
	return( 1 );

}

/*
** _compare_u(q1,q2)
**
** compare the key values from the supplied qnodes using the uint32_t member
**
** returns the status of the comparison:
**      < 0     q1 < q2
**      = 0     q1 == q2
**      > 0     q1 > q2
*/

int _compare_u( register qnode_t *q1, register qnode_t *q2 ) {

	// should really do a sanity check here, but what if
	// it fails?

	if( q1->key.u < q2->key.u ) return( -1 );
	else if( q1->key.u == q2->key.u ) return( 0 );
	return( 1 );

}

/*
** _q_init( void )
**
** initialize the queue module
*/

void _q_init( void ) {
	int i;
	
	// init the qnode list

	_free_qnodes = NULL;
	for( i = 0; i < N_QNODES; ++i ) {
		_qnode_free( &_qnodes[i] );
	}

	// system-wide key

	_key0.u = 0;
	
	// report that we have finished

	c_puts( " queues" );
}

/*
** _q_status(queue)
**
** evaluate the status of a queue
**
** returns the status
*/

status_t _q_status( queue_t *que ) {

	if( que == NULL ) {
		return( BAD_PARAM );
	}
	
	if( que->head == NULL ) {
		return( EMPTY_QUEUE );
	}

	return( NOT_EMPTY_QUEUE );
}

/*
** _q_reset(que,compare)
**
** reset the supplied queue by setting its qnode pointers to NULL
** and associating the supplied comparison routine with it
*/

void _q_reset( queue_t *que, int (*compare)(qnode_t*,qnode_t*) ) {

	// sanity check here?
	que->head = NULL;
	que->tail = NULL;
	que->compare = compare;

}

/*
** _q_insert(queue,data,key)
**
** insert the supplied data value into a queue, using the supplied
** key for ordering according to the queue's ordering rule
**
** returns the status of the insertion attempt
*/

status_t _q_insert( queue_t *que, void *data, key_t key ) {
	qnode_t *new, *curr;
	
	// sanity check
	//
	// note that inserting a NULL pointer is not prohibited

	if( que == NULL ) {
		return( BAD_PARAM );
	}
	
	// allocate a qnode for the insertion

	new = _qnode_alloc();
	if( new == NULL ) {
		return( NO_QNODES );
	}

	// copy the supplied data into the new qnode

	new->data = data;
	new->key = key;
	
	/*
	** handle the special cases of an empty queue and an unordered
	** queue here - in both cases, we just add at the end
	*/

	if( que->compare == NULL || _q_empty(que) ) {
		new->prev = que->tail;
		new->next = NULL;
		if( que->tail == NULL ) {
			que->head = new;
		} else {
			que->tail->next = new;
		}
		que->tail = new;
		return( SUCCESS );
	}
	
	/*
	** OK, we have a non-empty, ordered queue.  Locate the correct
	** insertion point.
	*/

	curr = que->head;
	while( curr != NULL && que->compare(new,curr) >= 0 ) {
		curr = curr->next;
	}

	/*
	** Here are the possible outcomes of the search:
	**
	** curr	 curr->prev	actions
	** ====	 ==========	===================================
	** NULL   ---		new->next = curr; // add at end
	**			new->prev = tail;
	**			tail->next = new;
	**			tail = new;
	**
	** !NULL  NULL		new->next = curr; // add at beginning
	**			new->prev = curr->prev;
	**			curr->prev = new;
	**			head = new;
	**
	** !NULL !NULL		new->next = curr; // add in middle
	**			new->prev = curr->prev;
	**			curr->prev->next = new;
	**			curr->prev = new;
	*/

	// always

	new->next = curr;

	if( curr == NULL ) {
		// add at the end of the list
		new->prev = que->tail;
		que->tail->next = new;
		que->tail = new;
	} else {
		// add earlier in the list
		new->prev = curr->prev;
		if( curr->prev == NULL ) {
			que->head = new;
		} else {
			curr->prev->next = new;
		}
		curr->prev = new;
	}
	
	return( SUCCESS );
}

/*
** _q_remove(queue,data)
**
** remove the first element from the queue, returning the pointer to
** it through the second parameter
**
** returns the status of the removal attempt
*/

status_t _q_remove( queue_t *que, void **data ) {
	qnode_t *node;
	
	// sanity check!

	if( que == NULL || data == NULL ) {
		return( BAD_PARAM );
	}
	
	// is there anything to remove?

	if( _q_empty(que) ) {
		return( EMPTY_QUEUE );
	}
	
	// non-empty queue; remove the first element

	node = que->head;

	// copy out the data pointer

	*data = node->data;
	
	// unlink this from the list

	que->head = node->next;
	if( que->head != NULL ) {
		que->head->prev = NULL;
	} else {
		que->tail = NULL;
	}
	
	// release the Kraken..., uh, qnode

	_qnode_free( node );
	
	return( SUCCESS );
}

/*
** _q_remove_specific(que,data,key)
**
** remove the element specified by the third parameter from the queue,
** returning the pointer to it through the second parameter
**
** returns the status of the removal attempt
**
** this is based largely on _q_remove()
*/

status_t _q_remove_specific( queue_t *que, void **data, key_t key ) {
	qnode_t *node;
	
	// sanity check!

	if( que == NULL || data == NULL ) {
		return( BAD_PARAM );
	}
	
	// is there anything to remove?

	if( _q_empty(que) ) {
		return( EMPTY_QUEUE );
	}
	
	// non-empty queue; locate the specified element

	node = que->head;
	while( node != NULL && node->key.u != key.u ) {
		node = node->next;
	}

	// did we find it?

	if( node == NULL ) {
		return( NOT_FOUND );
	}

	// yes - copy out the data pointer

	*data = node->data;
	
	// unlink this from the list

	if( node->prev != NULL ) {	// not the first element
		node->prev->next = node->next;
	} else {			// first element
		que->head = node->next;
	}

	if( node->next != NULL ) {	// not the last element
		node->next->prev = node->prev;
	} else {			// the last element
		que->tail = node->prev;
	}

	// release the Kraken..., uh, qnode

	_qnode_free( node );
	
	return( SUCCESS );
}

/*
** _q_dump(which,queue)
**
** dump the contents of the specified queue to the console
*/

void _q_dump( char *which, queue_t *queue ) {
	qnode_t *tmp;


	c_printf( "%s: ", which );
	if( queue == NULL ) {
		c_puts( "NULL???" );
		return;
	}

	c_printf( "head %08x tail %08x comp %08x\n",
		  (uint32_t) queue->head, (uint32_t) queue->tail,
		  (uint32_t) queue->compare );
	
	if( !_q_empty(queue) ) {
		c_puts( " keys: " );
		for( tmp = queue->head; tmp != NULL; tmp = tmp->next ) {
			c_printf( " [%x]", tmp->key.u );
		}
		c_puts( "\n" );
	}

}

/*
** _q_dump_all()
**
** dump the contents of all the queues
*/

void _q_dump_all( void ) {

	c_printf( "Queue contents @ %08x\n", _system_time );
	_q_dump( "ready[0]", &_ready[0] );
	_q_dump( "ready[1]", &_ready[1] );
	_q_dump( "ready[2]", &_ready[2] );
	_q_dump( "ready[3]", &_ready[3] );
	_q_dump( "sleep", &_sleeping );
	_q_dump( "read", &_reading );
	_q_dump( "wait", &_waiting );
	_q_dump( "zombie", &_zombie );
	_sio_dump();

}
