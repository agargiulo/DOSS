/*
** SCCS ID:	@(#)clock.c	1.1	4/2/13
**
** File:	clock.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Clock module implementation
*/

#define	__SP2_KERNEL__

#include "common.h"

#include <x86arch.h>
#include "startup.h"

#include "clock.h"
#include "process.h"
#include "queue.h"
#include "scheduler.h"
#include "sio.h"
#include "syscall.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

// pinwheel control variables

static uint32_t _pinwheel;	// pinwheel counter
static uint32_t _pindex;	// index into pinwheel string

/*
** PUBLIC GLOBAL VARIABLES
*/

time_t	_system_time;		// the current system time

/*
** PRIVATE FUNCTIONS
*/


/*
** _clock_isr(vector,code)
**
** Interrupt handler for the clock module.  Spins the pinwheel,
** wakes up sleeping processes, and handles quantum expiration
** for the current process.
*/

static void _clock_isr( int vector, int code ) {
	pcb_t *pcb;
	status_t status;

	// spin the pinwheel

	++_pinwheel;
	if( _pinwheel == (CLOCK_FREQUENCY / 10) ) {
		_pinwheel = 0;
		++_pindex;
		c_putchar_at( 79, 0, "|/-\\"[ _pindex & 3 ] );
	}

	// increment the system time

	++_system_time;

	/*
	** wake up any sleeper whose time has come
	**
	** we give awakened processes preference over the
	** current process (when it is scheduled again)
	*/

	while( !_q_empty(&_sleeping) &&
		_sleeping.head->key.t <= _system_time ) {
		
		// remove the first sleeper

		status = _q_remove( &_sleeping, (void **) &pcb );
		if( status != SUCCESS ) {
			_kpanic( "_clock_isr", "sleep remove status %s",
				 status );
		}
		
		// and put it in the ready queue
		_schedule( pcb );
	}
	
	// check the current process to see if it needs to be scheduled

	if( _current->quantum < 1 ) {
		_schedule( _current );
		_dispatch();
	} else {
		--(_current->quantum);
	}

	// Approximately every 20 seconds, dump the queues, and
	// print the contents of the SIO buffers.

	if( (_system_time % seconds_to_ticks(20)) == 0 ) {
		_q_dump_all();
	}

	// tell the PIC we're done

	__outb( PIC_MASTER_CMD_PORT, PIC_EOI );

}

/*
** PUBLIC FUNCTIONS
*/

/*
** _clock_init()
**
** initialize the clock module
*/

void _clock_init( void ) {
	uint32_t divisor;

	// start the pinwheel

	_pinwheel = _pindex = 0;

	// return to the epoch

	_system_time = 0;

	// set the clock to tick at CLOCK_FREQUENCY Hz.

	divisor = TIMER_FREQUENCY / CLOCK_FREQUENCY;
	__outb( TIMER_CONTROL_PORT, TIMER_0_LOAD | TIMER_0_SQUARE );
        __outb( TIMER_0_PORT, divisor & 0xff );        // LSB of divisor
        __outb( TIMER_0_PORT, (divisor >> 8) & 0xff ); // MSB of divisor

	// register the ISR

	__install_isr( INT_VEC_TIMER, _clock_isr );

        // announce that we have initialized the clock module

        c_puts( " clock" );
}
