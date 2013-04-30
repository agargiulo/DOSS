/*
** SCCS ID:	@(#)system.c	1.1	4/2/13
**
** File:	system.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Miscellaneous OS support implementations
*/

#define	__SP2_KERNEL__

#include <common.h>

#include <system.h>
#include <clock.h>
#include <process.h>
#include <bootstrap.h>
#include <syscall.h>
#include <sio.h>
#include <scheduler.h>
#include <pci.h>

#include <startup.h>
#include <x86arch.h>

// need init() address
#include <user.h>

// need the exit() prototype
#include <ulib.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

static pcb_t *_init_pcb;	// PCB of init() process

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _create_process(pcb)
**
** allocate and initialize a new process' data structures (PCB, stack)
**
** returns:
**      pointer to the new PCB
*/

pcb_t *_create_process( pcb_t *curr ) {
	pcb_t *pcb;
	stack_t *stack;
	int offset;
	uint32_t *ptr;
	
	// allocate the new structures

	pcb = _pcb_alloc();
	if( pcb == NULL ) {
		return( NULL );
	}

	stack = _stack_alloc();
	if( stack == NULL ) {
		_pcb_free(pcb);
		return( NULL );
	}
	
	/*
	** The PCB argument will be NULL if this function is called
	** from the system initialization, and non-NULL if called 
	** from the fork() implementation.
	**
	** In the former case, we initialize the new data structures for
	** a brand-new process.
	**
	** In the latter case, we replicate the information from the
	** existing process whose PCB was passed in.
	*/

	if( curr != NULL ) {	// called from fork()
	
		// duplicate the PCB and stack contents

		_kmemcpy( (void *) pcb, (void *) curr, sizeof(pcb_t) );
		_kmemcpy( (void *) stack, (void *) curr->stack, sizeof(stack_t) );
		
		// update the entries which should be changed in the PCB

		pcb->pid = _next_pid++;
		pcb->ppid = curr->pid;
		pcb->stack = stack;
		
	        /*
		** We duplicated the original stack contents, which
		** means that the context pointer and ESP and EBP values
		** in the new stack are still pointing into the original
		** stack.  We need to correct all of these.
		**
		** We have to change EBP because that's how the compiled
		** code for the user process accesses its local variables.
		** If we didn't change this, as soon as the new process
		** was dispatched, it would start to stomp on the local
		** variables in the original process' stack.  We also
       		** have to fix the EBP chain in the child process.
		**
		** None of this would be an issue if we were doing "real"
		** virtual memory, as we would be talking about virtual
		** addresses here rather than physical addresses, and all
		** processes would share the same virtual address space
		** layout.
		**
		** First, determine the distance (in bytes) between the
		** two stacks.  This is the adjustment value we must add
		** to the three pointers to correct them.  Note that this
		** distance may be positive or negative, depending on the
		** relative placement of the two stacks in memory.
		*/

		offset = (void *) (pcb->stack) - (void *) (curr->stack);
		
		// modify the context pointer for the new process

		pcb->context = (context_t *)( (void *) (pcb->context) + offset );
		// now, change ESP and EBP in the new process (easy to
		// do because they're just uint32_t values, not really
		// pointers)

		pcb->context->esp += offset;
		pcb->context->ebp += offset;

		/*
        	** Next, we must fix the EBP chain in the new stack.  This
		** is necessary in the situation where the fork() occurred
		** in a nested function call sequence; we fixed EBP, but
		** the "saved" EBP in the stack frame is pointing to the
		** calling function's frame in the original stack, not the
		** new stack.
		**
		** We are guaranteed that the chain of frames ends at the
		** frame for the original process' main() routine, because
		** exec() will initialize EBP for the process to 0, and the
		** entry prologue code in main() routine will push EBP,
		** ensuring a NULL pointer in the chain.
        	*/

		// start at the current frame

		ptr = (uint32_t *) pcb->context->ebp;

		// follow the chain of frame pointers to its end
		while( *ptr != 0 ) {
			// update the link from this frame to the previous
			*ptr += offset;
			// follow the updated link
			ptr = (uint32_t *) *ptr;
		}
		
	} else {		// called from init

		pcb->pid = pcb->ppid = PID_INIT;
		pcb->stack = stack;
	
	}
	
	// all done - return the new PCB

	return( pcb );
}

/*
** _create_stack(stack)
**
** initialize a fresh user context in a process stack
**
** returns:
**      pointer to the context save area in the stack
*/

context_t *_create_stack( stack_t *stack ) {
	uint32_t *ptr;
	context_t *context;

	// sanity check!

	if( stack == NULL ) {
		return( NULL );
	}

	// start by clearing the stack

	_kmemclr( (void *) stack, sizeof(stack_t) );

	/*
	** Set up the initial stack contents for a (new) user process.
	**
	** We reserve one longword at the bottom of the stack as
	** scratch space.  Above that, we simulate a call to exit() by
	** pushing the special exit status EXIT_DEFAULT and the address
	** of exit() as a "return address".  Finally, we simulate a call
	** from the entry point of exit() to main().  Above that, we
	** place an context_t area that is initialized with the
	** standard initial register contents.
	**
	** The low end of the stack will contain these values:
	**
	**      esp ->  ?     <- context save area
	**              ...   <- context save area
	**              ?     <- context save area
	**              exit  <- return address for faked call to main()
	**              0     <- return address for faked call to exit()
	**              code  <- special exit status
	**              0     <- last word in stack
	**
	** When this process is dispatched, the context restore code
	** will pop all the saved context information off the stack,
	** leaving the "return address" on the stack as if the main()
	** for the process had been "called" from the exit() stub.
	** When main() returns, it will "return" to the entry point of
	** exit(), which will clean it up.
	*/

	// first, find the address following the stack

	ptr = ((uint32_t *) (stack + 1)) - 2;

	// assign the filler data

	*--ptr = 0;

	// assign the default exit status

	*--ptr = EXIT_DEFAULT;

	// assign the return address for the faked call to exit()

	*--ptr = 0;

	// assign the return address for the faked call to main()

	*--ptr = (uint32_t) exit;

	// next, set up the process context

	context = ((context_t *) ptr) - 1;

	// initialize all the fields that should be non-zero, starting
	// with the segment registers

	context->cs = GDT_CODE;
	context->ss = GDT_STACK;
	context->ds = GDT_DATA;
	context->es = GDT_DATA;
	context->fs = GDT_DATA;
	context->gs = GDT_DATA;

	// EFLAGS must be set up to re-enable IF when we switch
	// "back" to this context

	context->eflags = DEFAULT_EFLAGS;

	/*
	** Note that we do *not* assign EIP here; we leave that
	** to the calling routine
	*/

	// all done - return the context pointer

	return( context );

}

/*
** _zombify(pcb)
**
** turn a process into a zombie, or give its status to
** a waiting parent
*/

void _zombify( pcb_t *pcb ) {
	estatus_t *esptr;
	pid_t *pidptr;
	pid_t ppid;
	pcb_t *parent, *p2;
	int i;
	key_t key;
	status_t stat;
	
	// mark the process as no longer runnable

	pcb->state = ZOMBIE;

	// find the parent

	ppid = pcb->ppid;
	for( i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].pid == ppid && _pcbs[i].state != FREE ) {
			parent = &_pcbs[i];
			break;
		}
	}
	
	/*
	** If we didn't find a parent, or if the parent was
	** already unrunnable (zombied, killed), reparent this
	** process to the init process
	*/

	if( i >= N_PCBS || _pcbs[i].state >= FIRST_DEAD_STATE ) {
		ppid = pcb->ppid = PID_INIT;
		parent = _init_pcb;
	}

	/*
	** At this point, parent points to the parent's PCB, and ppid
	** contains the parent's PID.
	**
	** If the parent is on the wait() queue, we'll awaken it and give
	** it this child's information.
	**
	** Otherwise, we need to put this child on the zombie queue.
	*/

	if( parent->state == WAITING ) {

		// look for the parent on the wait queue

		key.u = ppid;
		stat = _q_remove_specific( &_waiting, (void **) &p2, key );
		if( stat != SUCCESS ) {
			_kpanic( "_zombify", "parent wait remove status %s",
				 stat );
		}

		// verify that we found the same process

		if( p2 != parent ) {
			_pcb_dump( "*** p2:     ", p2 );
			_pcb_dump( "*** parent: ", parent );
			_kpanic( "_zombify", "parent wait deque wrong",
				 FAILURE );
		}

		// OK, we have the right one.

		// Start by decrementing its "remaining children"
		// counter if it isn't the init process

		if( ppid != PID_INIT ) {
			parent->children -= 1;
		}
		
		// return the child's information to it.

		RET(parent) = U_SUCCESS;

		pidptr = (pid_t *) ARG(parent)[1];
		*pidptr = pcb->pid;

		esptr = (estatus_t *) ARG(parent)[2];
		*esptr = ARG(pcb)[1];
		
		// schedule the parent (who returns from wait())

		_schedule( parent );
		
		// clean up the child process
		_pcb_cleanup( pcb );

	} else {

		// place this child on the zombie queue, ordered by PID

		key.u = pcb->pid;
		pcb->status = ARG(pcb)[1];

		stat = _q_insert( &_zombie, (void *)pcb, key );
		if( stat != SUCCESS ) {
			_kpanic( "_zombify", "zombie insert status %s",
				 stat );
		}

	}
	
}

/*
** _ignore_isr(vector,code)
**
** "Ignore this interrupt" handler - ACKs the interrupt, but
** does no other processing.
**
** this can be used, e.g., to handle interrupt 0x2a that occurs
** when a flash drive is removed from the USB port.
*/
void _ignore_isr( int vector, int code ){

	__outb( PIC_MASTER_CMD_PORT, PIC_EOI );
	if( vector >= 0x28 && vector <= 0x2f ) {
		__outb( PIC_SLAVE_CMD_PORT, PIC_EOI );
	}

}

/*
** _init - system initialization routine
**
** Called by the startup code immediately before returning into the
** first user process.
*/

void _init( void ) {
	pcb_t *pcb;

	/*
	** BOILERPLATE CODE - taken from basic framework
	**
	** Initialize interrupt stuff.
	*/

	__init_interrupts();	// IDT and PIC initialization

	/*
	** Console I/O system.
	*/

	c_io_init();

	/*
	** 20123-SPECIFIC CODE STARTS HERE
	*/

	/*
	** Initialize various OS modules
	**
	** Note:  the clock, SIO, and syscall modules also install
	** their ISRs.
	*/

	c_puts( "Module init: " );

	_q_init();		// must be first
	_pcb_init();
	_stack_init();
	_sio_init();
	_sys_init();
	_sched_init();
	_clock_init();
	_pci_init();

	c_puts( "\n" );

	_video_test();

	/*
	** Create the initial system ESP
	**
	** This will be the address of the next-to-last
	** longword in the system stack.
	*/

	_system_esp = ((uint32_t *) ( (&_system_stack) + 1)) - 2;

	/*
	** Create the initial process
	**
	** Code mostly stolen from _sys_fork(); if that routine
	** changes, SO MUST THIS!!!
	*/

	// allocate a PCB and stack

	pcb = _create_process( NULL );
	if( pcb == NULL ) {
		_kpanic( "_init", "init() creation failed", FAILURE );
	}

	// initialize the stack with the standard context

	pcb->context = _create_stack( pcb->stack );
	if( pcb->context == NULL ) {
		_kpanic( "_init", "init() stack setup failed", FAILURE );
	}

	// define the entry point for init()

	pcb->context->eip = (uint32_t) init;

	// set up various PCB fields

	pcb->pid = pcb->ppid = PID_INIT;	// next PID is initially 1
	pcb->prio = PRIO_HIGH;
	pcb->children = 1000;

	// remember this PCB for use in reparenting orphan processes

	_init_pcb = pcb;

	// make it the first process

	_schedule( pcb );
	_dispatch();

	/*
	** Turn on the SIO receiver (the transmitter will be turned
	** on/off as characters are being sent)
	*/

	_sio_enable( SIO_RX );

	/*
	** END OF 20123-SPECIFIC CODE
	**
	** Finally, report that we're all done.
	*/

	c_puts( "System initialization complete.\n" );

}
