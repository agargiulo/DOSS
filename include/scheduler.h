/*
** SCCS ID:	@(#)scheduler.h	1.1	4/2/13
**
** File:	scheduler.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Scheduler/dispatcher module declarations
*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

#include <process.h>
#include <queue.h>

// number of ready queues - one per priority level

#define	N_READYQ	N_PRIOS

/*
** Types
*/

/*
** Globals
*/

extern pcb_t *_current;		// the currently-running process
extern queue_t _ready[];	// the MLQ ready queue structure

/*
** Prototypes
*/

/*
** _sched_init()
**
** initialize the scheduler module
*/

void _sched_init( void );

/*
** _schedule(pcb)
**
** schedule a process for execution according to its priority
*/

void _schedule( pcb_t *pcb );

/*
** _dispatch()
**
** give the CPU to a process
*/

void _dispatch( void );

#endif

#endif
