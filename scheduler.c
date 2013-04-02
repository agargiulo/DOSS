/*
** SCCS ID:	@(#)scheduler.c	1.1	4/2/13
**
** File:	scheduler.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Scheduler/dispatcher implementation
*/

#define	__SP2_KERNEL__

#include "common.h"

#include "scheduler.h"

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

pcb_t *_current;		// the currently-running process
queue_t _ready[N_READYQ];	// the MLQ ready queue structure

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _sched_init()
**
** initialize the scheduler module
*/

void _sched_init( void ) {
	int i;
	
	// reset all the MLQ levels

	for( i = 0; i < N_READYQ; ++i ) {
		_q_reset( _ready + i, NULL );
	}
	
	// no current process, initially

	_current = NULL;
	
	// report that we have finished

	c_puts( " sched" );
}

/*
** _schedule(pcb)
**
** schedule a process for execution according to its priority
*/

void _schedule( pcb_t *pcb ) {
	status_t stat;
	key_t key;
	
	// sanity check:  should we verify that we don't have a NULL pointer?

	// verify the priority value for this process

	if( pcb->prio >= N_PRIOS ) {
		pcb->prio = PRIO_DEFERRED;
	}
	
	// mark this process as ready

	pcb->state = READY;

	// for now, use a standard quantum for all processes (this
	// could easily be modified to allow varying quanta)

	pcb->quantum = STD_QUANTUM;

	// idea:  use the PID as the key - this allows us to
	// easily search for a specific process if we need to

	key.u = pcb->pid;

	// add it to the appropriate ready queue level

	stat = _q_insert( _ready + pcb->prio, (void *)pcb, key );
	if( stat != SUCCESS ) {
		_kpanic( "_schedule", "insert status %s", stat );
	}
}

/*
** _dispatch()
**
** give the CPU to a process
*/

void _dispatch( void ) {
	pcb_t *pcb;
	int i;
	status_t stat;
	
	// select a process from the highest-priority
	// ready queue that is not empty

	while( 1 ) {

		// locate a non-empty queue

		for( i = 0; i < N_READYQ; ++i ) {
			if( !_q_empty(_ready + i) ) {
				break;
			}
		}

		// did we find one?

		if( i >= N_READYQ ) {
			_kpanic( "_dispatch", "all queues empty", FAILURE );
		}
		
		// remove the first process from it

		stat = _q_remove( &_ready[i], (void **) &pcb );
		if( stat != SUCCESS ) {
			_kpanic( "_dispatch", "queue remove status %s", stat );
		}

		// if this process is ready, we're all set

		if( pcb->state == READY ) {
			break;
		}

		// nope - see if it needs to be zombified

		if( pcb->state == KILLED ) {
			_zombify( pcb );
		} else {
			// not ready + not killed = bad news!
			_kpanic( "_dispatch", "non-ready, state(+100k)",
				 pcb->state + 100000 );
		}

	}

	pcb->state = RUNNING;
	_current = pcb;
}
