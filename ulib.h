/*
** SCCS ID:	@(#)ulib.h	1.1	4/2/13
**
** File:	ulib.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	User-level library declarations
**
** These are separate from the kernel library so that user processes
** aren't trying to use code that is in the kernel's address space,
** in case anyone wants to implement real memory protection.
*/

#ifndef _ULIB_H_
#define _ULIB_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

#include "process.h"

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

// ustatus_t value strings

extern const char *ustatus_strings[];

// estatus_t value strings

extern const char *estatus_strings[];

/*
** Prototypes
*/

/*
** exit - terminate the calling process
**
** usage:	exit(code);
**
** does not return
*/

ustatus_t exit( estatus_t code );

/*
** read - read a single character from the SIO
**
** usage:	status = read(&buf);
**
** blocks the calling routine if there is no character to return
**
** returns:
**      status of the operation
*/

ustatus_t read( char *buf );

/*
** write - write a single character to the SIO
**
** usage:	status = write(ch);
**
** returns:
**      status of the operation
*/

ustatus_t write( char ch );

/*
** fork - create a new process at a specific priority
**
** usage:  pid = fork(prio);
**
** returns:
**	-1 in parent if the fork() fails, else PID of new process
**	0 in child
*/

int fork( prio_t prio );

/*
** exec - replace a process with a different program
**
** usage:	status = exec(entry);
**
** returns:
**	does not return (if the attempt succeeds)
**      failure status of the replacement attempt (if the attempt fails)
*/

ustatus_t exec( void (*entry)(void) );

/*
** sleep - put the current process to sleep for some length of time
**
** usage:	status = sleep(ms);
**
** if the sleep time (in milliseconds) is 0, just preempts the process;
** otherwise, puts it onto the sleep queue for the specified length of
** time
**
** returns:
**      status of the sleep attempt
*/

ustatus_t sleep( uint_t ms );

/*
** wait - get exit status of a child process
**
** usage:	status = wait(&pid,&status);
**
** if there are no children in the system, returns an error status
**
** if there are one or more children in the system and at least one has
** terminated but hasn't yet been cleaned up, cleans up that process and
** returns its information; otherwise, blocks until a child terminates
**
** returns:
**      the PID and exit status of the child through the pointer parameters
**	the status of the wait request
*/

ustatus_t wait( pid_t *pid, estatus_t *status );

/*
** kill - terminate a process with extreme prejudice
**
** usage:	status = kill(pid);
**
** returns:
**      status of the termination attempt
*/

ustatus_t kill( pid_t pid );

/*
** getprio - retrieve the priority of the specified process
**
** usage:	status = getprio(pid,&prio);
**
** returns:
**      the process' priority through the second parameter
**      status of the request
*/

ustatus_t getprio( pid_t pid, prio_t *prio );

/*
** setprio - change the priority of the specified process
**
** usage:	status = setprio(pid,prio);
**
** returns:
**      status of the request
*/

ustatus_t setprio( pid_t pid, prio_t prio );

/*
** getpid - retrieve the PID of the current process
**
** usage:	pid = getpid();
**
** returns:
**      the process' pid
*/

pid_t getpid( void );

/*
** getppid - retrieve the parent PID of the current process
**
** usage:	pid = getppid();
**
** returns:
**      the process' parent's pid
**
** note: if the original parent of this process has exited, this process
** will have been inherited by the system init process, whose PID will
** be returned
*/

pid_t getppid( void );

/*
** gettime - retrieve the current system time
**
** usage:	time = gettime();
**
** returns:
**      the current system time
*/

time_t gettime( void );

/*
** bogus - a bogus system call, for testing our syscall ISR
**
** usage:	bogus();
*/

void bogus( void );

/*      
** prt_ustat - print a ustatus_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/
        
void prt_ustat( char *msg, ustatus_t stat );

/*      
** prt_estat - print an estatus_t value to the console
**      
** the 'msg' argument should contain a %s where
** the desired status value should be printed
*/
        
void prt_estat( char *msg, estatus_t stat );

#endif

#endif
