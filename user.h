/*
** SCCS ID:	@(#)user.h	1.1	4/2/13
**
** File:	user.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	User routines
*/

#ifndef _USER_H_
#define _USER_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

// delay loop counts

#define	DELAY_LONG	100000000
#define	DELAY_STD	  2500000
#define	DELAY_ALT	  4500000

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** User process controls.
**
** The comment field of these definitions contains a list of the
** system calls this user process uses.
**
** To spawn a specific user process from the initial process,
** uncomment its entry in this list.
*/

#define	SPAWN_A   // write, exit
#define	SPAWN_B   // write, exit
#define	SPAWN_C   // write, exit
#define	SPAWN_D   // write, fork, exec, exit
#define	SPAWN_E   // getpid, write, sleep, exit
#define	SPAWN_F   // getpid, write, sleep, exit
#define	SPAWN_G   // getpid, write, sleep, exit
#define	SPAWN_H   // write, sleep
// no user i
#define	SPAWN_J   // write, fork, exec, exit
#define	SPAWN_K   // write, fork, exec, sleep, exit
#define	SPAWN_L   // gettime, write, fork, exec, sleep, exit
#define	SPAWN_M   // fork, exec, write, exit
#define	SPAWN_N   // fork, exec, write, exit
// no user O
#define	SPAWN_P   // gettime, sleep, write, exit
#define	SPAWN_Q   // write, bogus, exit
#define	SPAWN_R   // sleep, write, read, exit
#define	SPAWN_S   // write, sleep, exit
#define	SPAWN_T   // getpid, getprio, write, setprio, exit
#define	SPAWN_U   // fork, exec, write, sleep, kill, exit
// no user V

/*
** Users W-Z are spawned from other processes; they
** should never be spawned directly.
*/

// user W:  getpid, write, sleep, exit
// user X:  getpid, write, exit
// user Y:  getpid, write, sleep, exit
// user Z:  getpid, getppid, write, exit


/*
** Prototypes for externally-visible routines
*/

/*
** init - initial user process
**
** after spawning the other user processes, loops forever calling wait()
*/

void init( void );

#endif

#endif
