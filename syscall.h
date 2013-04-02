/*
** SCCS ID:	@(#)syscall.h	1.1	4/2/13
**
** File:	syscall.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	System call module definitions
*/

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

// system call codes

#define	SYS_exit		0
#define	SYS_read		1
#define	SYS_write		2
#define	SYS_fork		3
#define	SYS_exec		4
#define	SYS_sleep		5
#define	SYS_wait		6
#define	SYS_kill		7
#define	SYS_getprio		8
#define	SYS_setprio		9
#define	SYS_getpid		10
#define	SYS_getppid		11
#define	SYS_gettime		12

// number of "real" system calls

#define	N_SYSCALLS	14

// dummy system call code to test the syscall ISR

#define	SYS_bogus	0xbadc0de

// system call interrupt vector number

#define	INT_VEC_SYSCALL	0x80

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

#ifdef __SP2_KERNEL__

#include "queue.h"

#include <x86arch.h>

/*
** OS only definitions
*/

// default contents of EFLAGS register

#define	DEFAULT_EFLAGS	(EFLAGS_MB1 | EFLAGS_IF)

/*
** PUBLIC GLOBAL VARIABLES
*/

extern queue_t _sleeping;      // sleeping processes
extern queue_t _waiting;       // waiting for exit status information
extern queue_t _zombie;        // exited but not collected processes

/*
** Prototypes
*/

/*
** _sys_init()
**
** initializes all syscall-related data structures
*/

void _sys_init( void );

#endif

#endif

#endif
