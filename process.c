/*
** SCCS ID:	@(#)process.c	1.1	4/2/13
**
** File:	process.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Process-related implementations
*/

#define	__SP2_KERNEL__

#include <common.h>

#include <process.h>
#include <queue.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

static queue_t _free_pcbs;	// queue of available PCBs

/*
** PUBLIC GLOBAL VARIABLES
*/

pcb_t _pcbs[ N_PCBS ];		// all the PCBs in the system
pid_t _next_pid;		// next available PID

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _pcb_init()
**
** initializes all pcb-related data structures
*/

void _pcb_init( void ) {
	int i;
	
	// clear the free PCB queue

	_q_reset( &_free_pcbs, NULL );
	
	// "free" all the PCBs

	for( i = 0; i < N_PCBS; ++i ) {
		_pcb_free( &_pcbs[i] );
	}
	
	// set the initial PID
	// (must be the PID of the init() process)

	_next_pid = PID_FIRST;

	// report that we have finished

	c_puts( " pcbs" );
}

/*
** _pcb_alloc()
**
** allocate a pcb structure
**
** returns a pointer to the pcb, or NULL on failure
*/

pcb_t *_pcb_alloc( void ) {
	pcb_t *pcb;
	status_t stat;
	
	// pull the first available PCB off the free queue

	stat = _q_remove( &_free_pcbs, (void **) &pcb );
	if( stat == BAD_PARAM ) {
		_kpanic( "_pcb_alloc", "queue remove status %s", stat );
	} else if( stat == EMPTY_QUEUE ) {
		return( NULL );
	}

	// verify that it's not in use

	if( pcb->state != FREE ) {
		_kpanic( "_pcb_alloc", "removed non-free PCB", FAILURE );
	}
	
	// return it to the caller (or NULL)

	return( pcb );
}

/*
** _pcb_free(pcb)
**
** deallocate a pcb, putting it into the list of available pcbs
*/

void _pcb_free( pcb_t *pcb ) {
	status_t stat;
	
	// sanity check:  avoid deallocating a NULL pointer
	if( pcb == NULL ) {
		// should this be an error?
		return;
	}

	// return the PCB to the free list

	pcb->state = FREE;
	stat = _q_insert( &_free_pcbs, (void *) pcb, _key0 );
	if( stat != SUCCESS ) {
		_kpanic( "_pcb_free", "free queue insert status %s", stat );
	}
}

/*
** _pcb_cleanup(pcb)
**
** reclaim a process' data structures
*/

void _pcb_cleanup( pcb_t *pcb ) {

	// sanity check:  avoid deallocating a NULL pointer
	if( pcb == NULL ) {
		// should this be an error?
		return;
	}

	// release the stack
	if( pcb->stack != NULL ) {
		_stack_free( pcb->stack );
	}

	// release the PCB
	_pcb_free( pcb );
}

/*
** _pcb_dump(pcb)
**
** dump the contents of this PCB to the console
*/

void _pcb_dump( const char *which, pcb_t *pcb ) {

	c_printf( "%s: ", which );
	if( pcb == NULL ) {
		c_puts( " NULL???\n" );
		return;
	}

	c_printf( " pids %d/%d state ", pcb->pid, pcb->ppid );
	switch( pcb->state ) {
		case FREE:	c_puts( "FREE" ); break;
		case NEW:	c_puts( "NEW" ); break;
		case READY:	c_puts( "READY" ); break;
		case RUNNING:	c_puts( "RUNNING" ); break;
		case SLEEPING:	c_puts( "SLEEPING" ); break;
		case BLOCKED:	c_puts( "BLOCKED" ); break;
		case WAITING:	c_puts( "WAITING" ); break;
		case KILLED:	c_puts( "KILLED" ); break;
		case ZOMBIE:	c_puts( "ZOMBIE" ); break;
		default:	c_printf( "? (%d)", pcb->state );
	}

	c_puts( " prio " );
	switch( pcb->prio ) {
		case PRIO_HIGH:		c_puts( "HIGH" ); break;
		case PRIO_STD:		c_puts( "STD" ); break;
		case PRIO_LOW:		c_puts( "LOW" ); break;
		case PRIO_DEFERRED:	c_puts( "DEF" ); break;
		default:	c_printf( "? (%d)", pcb->prio );
	}

	c_printf( "\n q %d estat %d wake %08x", pcb->quantum,
		  pcb->status, pcb->wakeup );

	c_printf( " context %08x stack %08x\n",
		  (uint32_t) pcb->context, (uint32_t) pcb->stack );
}
