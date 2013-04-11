/*
** SCCS ID:	@(#)syscall.c	1.1	4/2/13
**
** File:	syscall.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	System call module implementation
*/

#define	__SP2_KERNEL__

#include <common.h>

#include <syscall.h>

#include <process.h>
#include <stack.h>
#include <queue.h>
#include <scheduler.h>
#include <sio.h>

#include <support.h>
#include <startup.h>
#include <x86arch.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

// system call jump table
//
// initialized by _sys_init() to ensure that
// code::function mappings are correct

static void (*_syscalls[ N_SYSCALLS ])( pcb_t *);

/*
** PUBLIC GLOBAL VARIABLES
*/

queue_t _sleeping;	// sleeping processes
queue_t _waiting;	// waiting for exit status information
queue_t _zombie;	// exited but not collected processes

/*
** PRIVATE FUNCTIONS
*/

/*
** _sys_isr(vector,code)
**
** Common handler for the system call module.  Selects
** the correct second-level routine to invoke based on
** the contents of EAX.
**
** The second-level routine is invoked with a pointer to
** the PCB for the process.  It is the responsibility of
** that routine to assign all return values for the call.
*/

static void _sys_isr( int vector, int code ) {
	uint_t syscode;

	// sanity check - verify that there is a current process!

	if( _current == NULL ) {
		_kpanic( "_sys_isr", "null _current!", FAILURE );
	}

	// also, verify that it has a context

	if( _current->context == NULL ) {
		_kpanic( "_sys_isr", "null _current context!", FAILURE );
	}

	// retrieve the syscall code

	syscode = RET(_current);

	// verify that it's legal - if not, force an exit(EXIT_BAD_SYSCALL)

	if( syscode >= N_SYSCALLS ) {
		syscode = SYS_exit;
		ARG(_current)[1] = EXIT_BAD_SYSCALL;
	}

	// call the handler

	(*_syscalls[syscode])(_current);

	// tell the PIC we're done

	__outb( PIC_MASTER_CMD_PORT, PIC_EOI );
}

/*
** Second-level syscall handlers
**
** All have this prototype:
**
**      static void _sys_NAME( pcb_t * );
**
** Most syscalls return a status_t value to the user calling function.
** Those which return additional information from the system have as
** their first user-level argument a pointer (called the "info pointer"
** below) to a variable into which the information is to be placed.
*/

/*
** _sys_exit - terminate the calling process
**
** implements:  void exit(code);
**
** does not return
*/

static void _sys_exit( pcb_t *pcb ) {
	
	// assign the exit status

	pcb->status = ARG(pcb)[1];
	
	// see if our parent is waiting for us; either give it
	// our status and clean us up, or turn us into a zombie

	_zombify( pcb );

	// pick a new current process

	_dispatch();
}

/*
** _sys_read - read a single character from the SIO
**
** implements:	status_t read(char *buf);
**
** blocks the calling routine if there is no character to return
**
** returns:
**	the character via the info pointer
**	status of the operation
*/

static void _sys_read( pcb_t *pcb ) {
	key_t key;
	int ch;
	char *ptr;
	status_t stat;

	// try to get the next character

	ch = _sio_readc();

	// if there was a character, return it to the process;
	// otherwise, block the process until one comes in

	if( ch >= 0 ) {

		ptr = (char *) (ARG(pcb)[1]);
		*ptr = ch;
		RET(pcb) = SUCCESS;

	} else {

		// no character; put this process on the
		// serial i/o input queue

		_current->state = BLOCKED;

		key.u = _current->pid;
		stat = _q_insert( &_reading, (void *) _current, key );
		if( stat != SUCCESS ) {
			_kpanic( "_sys_read", "insert status %s", stat );
		}

		// select a new current process

		_dispatch();

	}


}

/*
** _sys_write - write a single character to the SIO
**
** implements:	status_t write(char buf);
**
** returns:
**	status of the operation
*/

static void _sys_write( pcb_t *pcb ) {
	int ch = ARG(pcb)[1];

	// this is almost insanely simple, but it does separate
	// the low-level device access fromm the higher-level
	// syscall implementation

	_sio_writec( ch );

	RET(pcb) = SUCCESS;

}

/*
** _sys_fork - create a new process
**
** implements:  status_t fork(pid_t *pid);
**
** returns on success:
**      PID of new process (in parent)
**      0 (in child)
**
** returns on failure:
**	-1 (in parent)
*/

static void _sys_fork( pcb_t *pcb ) {
	pcb_t *new;
	
	// replicate the current PCB and stack

	new = _create_process( pcb );

	if( new == NULL ) {
		RET(pcb) = -1;
		return;
	}

	// update fields in the new process

	new->ppid = pcb->pid;
	new->prio = ARG(pcb)[1];
	new->children = 0;
	RET(new) = 0;

	// parent has one more child (except for init)

	if( pcb->pid != PID_INIT ) {
		pcb->children += 1;
	}
	RET(pcb) = new->pid;
	
	/*
	** Philosophical issue:  should the child run immediately, or
	** should the parent continue?
	**
	** We take the path of least resistance (work), and opt for the
	** latter; we schedule the child, and let the parent continue.
	*/

	_schedule( new );
}

/*
** _sys_exec - replace a process with a different program
**
** implements:  status_t exec(void (*entry)(void));
**
** returns:
**      does not return (if the attempt succeeds)
**      failure status of the attempt (if the attempt fails)
**
** note that in our baseline system, this call cannot fail
*/

static void _sys_exec( pcb_t *pcb ) {
	uint32_t entry;
	
	// get the entry point of the new program

	entry = ARG(pcb)[1];

	// now, reset the stack of the process

	pcb->context = _create_stack( pcb->stack );
	
	// assign the new entry point

	pcb->context->eip = entry;
}

/*
** _sys_execv - replace a process with a different program
**
** implements:  status_t execv(void (*entry)(void), int argc, char **argv);
**
** returns:
**      does not return (if the attempt succeeds)
**      failure status of the attempt (if the attempt fails)
**
** note that in our baseline system, this call cannot fail
*/

static void _sys_execv( pcb_t *pcb ) {
	uint32_t entry;
	int argc;
	char **argv;
	
	// get the entry point of the new program

	entry = ARG(pcb)[1];
	argc = ARG(pcb)[2];
	argv = ARG(pcb)[3];

	c_printf("execv: %d %d %d\n", entry, argc, argv);
	// now, reset the stack of the process

	pcb->context = _create_stack( pcb->stack );
	
	// assign the new entry point

	pcb->context->eip = entry;
	
	ARG(pcb)[1] = argc;
	ARG(pcb)[2] = argv;
	
}


/*
** _sys_sleep - put the current process to sleep for some length of time
**
** implements:	status_t sleep(uint32_t ms);
**
** if the sleep time (in milliseconds) is 0, just preempts the process;
** otherwise, puts it onto the sleep queue for the specified length of
** time
**
** returns:
**	status of the sleep attempt
*/

static void _sys_sleep( pcb_t *pcb ) {
	uint_t ms;
	key_t key;
	status_t stat;
	
	// convert the sleep time from ms into clock ticks

	ms = ARG(pcb)[1];
	ms = ms_to_ticks(ms);
	
	// sleep() rarely fails

	RET(pcb) = U_SUCCESS;
	

	if( ms == 0 ) {

		// just preempt the process

		_schedule( pcb );

	} else {

		// calculate the wakeup time for the process
		pcb->wakeup = _system_time + ms;

		// use that to order the sleep queue
		key.t = pcb->wakeup;

		stat = _q_insert( &_sleeping, (void *)pcb, key );
		if( stat != SUCCESS ) {
			_kpanic( "_sys_sleep", "sleep insert status %s",
				  stat );
		}

	}
	
	// no current process - pick another one

	_dispatch();
}

/*
** _sys_wait - wait for one of the children of a process
**
** implements:	status_t wait(pid_t *pid, estatus_t *stat);
**
** returns:
**	the PID and exit status of the child
**	the status of the request
*/

static void _sys_wait( pcb_t *pcb ) {
	int i;
	pcb_t *c1, *c2;
	key_t key;
	status_t stat;

	// verify that there is at least one child still in existance

	if( pcb->pid != PID_INIT && pcb->children < 1 ) {
		RET(pcb) = U_NOT_FOUND;
		return;
	}

	/*
	** Locate a child process that is either on the zombie queue
	** (c1) or still executing (c2).  Keep a pointer to its PCB
	** so that we can collect its information.
	*/

	c1 = c2 = NULL;
	for( i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].ppid == pcb->pid && _pcbs[i].state != FREE ) {
			if( _pcbs[i].state == ZOMBIE ) {
				c1 = &_pcbs[i];
			} else {
				c2 = &_pcbs[i];
			}
		}
	}

	// did we find a zombie?  if so, collect its information and return

	if( c1 != NULL ) {
		pid_t *pidptr;
		estatus_t *statptr;
		pcb_t *ptr;
		status_t stat;
		key_t key;
		
		// collect the PID and exit status pointers

		pidptr = (pid_t *) ARG(pcb)[1];
		statptr = (estatus_t *) ARG(pcb)[2];	
		
		// pull this process off the zombie queue

		key.u = c1->pid;
		stat = _q_remove_specific( &_zombie, (void **) &ptr, key );
		if( stat != SUCCESS ) {
			c_printf( "*** _sys_wait, parent %d zombie child %d\n",
				   pcb->pid, c1->pid );
			_pcb_dump( "zombie pcb", c1 );
			_kpanic( "_sys_wait",
				 "zombie remove failed - %s", stat );
		}
		
		// copy the requested data into the caller

		*pidptr = c1->pid;
		*statptr = c1->status;

		// one fewer child for the caller
		pcb->children -= 1;
		
		// clean up the child

		_pcb_cleanup( c1 );	// WRITE THIS!!!
		
		// we're done

		RET(pcb) = U_SUCCESS;
		return;
	}

	// no zombie child; verify that there's still an active child

	if( c2 == NULL ) {
		// this shouldn't be able to happen
		_pcb_dump( "waiting parent", pcb );
		_kpanic( "_sys_wait", "no children found", FAILURE );
	}

	// found an active child, so we'll block until it (or another) exits

	key.u = pcb->pid;
	stat = _q_insert( &_waiting, (void *) pcb, key );
	if( stat != SUCCESS ) {
		_kpanic( "_sys_wait", "wait insert status %s", stat );
	}

	pcb->state = WAITING;

	// we're blocked, so select a new current process

	_dispatch();
}

/*
** _sys_kill - terminate a process with extreme prejudice
**
** implements:	status_t kill(pid_t pid);
**
** returns:
**	status of the termination attempt
*/

static void _sys_kill( pcb_t *pcb ) {
	pid_t pid;
	int i;

	// locate our victim's PCB

	pid = ARG(pcb)[1];
	for( i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].pid == pid && _pcbs[i].state != FREE ) {
			break;
		}
	}

	// if we didn't find it, tell our caller

	if( i >= N_PCBS ) {
		RET(pcb) = U_NOT_FOUND;
		return;
	}

	// found it - mark it as killed if it isn't already dead

	if( _pcbs[i].state < FIRST_DEAD_STATE ) {
		RET(pcb) = U_ALREADY_DEAD;
	} else {
		_pcbs[i].state = KILLED;
		RET(pcb) = U_SUCCESS;
	}
}

/*
** _sys_getprio - retrieve the priority of the specified process
**
** implements:	status_t getprio(pid_t pid, prio_t *prio);
**
** returns:
**	the process' priority via the info pointer
**	the status of the request
*/

static void _sys_getprio( pcb_t *pcb ) {
	int i;
	pid_t pid;
	prio_t *ptr;

	// locate our target process

	pid = ARG(pcb)[1];
	for( i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].pid == pid && _pcbs[i].state != FREE ) {
			break;
		}
	}

	// did we find it?

	if( i < N_PCBS ) {
		// yes - collect the data
		RET(pcb) = U_SUCCESS;
		ptr = (prio_t *) ARG(pcb)[2];
		*ptr = _pcbs[i].prio;
	} else {
		// no - return a failure indicator
		RET(pcb) = U_NOT_FOUND;
	}
}

/*
** _sys_setprio - change the priority of the specified process
**
** implements:	status_t setprio(pid_t pid, prio_t prio);
**
** returns:
**	the status of the request
*/

static void _sys_setprio( pcb_t *pcb ) {
	int i;
	pid_t pid;
	prio_t prio;
		
	// locate our target process

	pid = ARG(pcb)[1];
	for( i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].pid == pid && _pcbs[i].state != FREE ) {
			break;
		}
	}

	// did we find it?

	if( i < N_PCBS ) {
		// yes - collect the data
		RET(pcb) = U_SUCCESS;
		prio = ARG(pcb)[2];
		_pcbs[i].prio = prio;
	} else {
		// no - return a failure indicator
		RET(pcb) = U_NOT_FOUND;
	}
}

/*
** _sys_getpid - retrieve the PID of the current process
**
** implements:	pid_t getpid();
**
** returns:
**	the process' pid
*/

static void _sys_getpid( pcb_t *pcb ) {
	RET(pcb) = pcb->pid;
}

/*
** _sys_getppid - retrieve the parent PID of the current process
**
** implements:	pid_t getppid();
**
** returns:
**	the process' parent's pid
*/

static void _sys_getppid( pcb_t *pcb ) {
	RET(pcb) = pcb->ppid;
}

/*
** _sys_gettime - retrieve the current system time
**
** implements:	time_t gettime();
**
** returns:
**	the current system time
*/

static void _sys_gettime( pcb_t *pcb ) {
	RET(pcb) = _system_time;
}

/*
** PUBLIC FUNCTIONS
*/

/*
** _sys_init()
**
** initialize the syscall module
*/

void _sys_init( void ) {

	// initialize the sleep and wait queues

	_q_reset( &_sleeping, _compare_t );
	_q_reset( &_waiting, _compare_u );
	_q_reset( &_zombie, _compare_u );

	/*
	** Set up the syscall jump table.  We do this here
	** to ensure that the association between syscall
	** code and function address is correct even if the
	** codes change.
	*/

	_syscalls[ SYS_exit ]      = _sys_exit;
	_syscalls[ SYS_read ]      = _sys_read;
	_syscalls[ SYS_write ]     = _sys_write;
	_syscalls[ SYS_fork ]      = _sys_fork;
	_syscalls[ SYS_exec ]      = _sys_exec;
	_syscalls[ SYS_execv ]     = _sys_execv;
	_syscalls[ SYS_sleep ]     = _sys_sleep;
	_syscalls[ SYS_wait ]      = _sys_wait;
	_syscalls[ SYS_kill ]      = _sys_kill;
	_syscalls[ SYS_getprio ]   = _sys_getprio;
	_syscalls[ SYS_setprio ]   = _sys_setprio;
	_syscalls[ SYS_getpid ]    = _sys_getpid;
	_syscalls[ SYS_getppid ]   = _sys_getppid;
	_syscalls[ SYS_gettime ]   = _sys_gettime;

	// install our ISR

	__install_isr( INT_VEC_SYSCALL, _sys_isr );

	c_puts( " syscall" );
}
