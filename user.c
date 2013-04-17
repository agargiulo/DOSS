/*
** SCCS ID:	@(#)user.c	1.1	4/2/13
**
** File:	user.c
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	User routines.
*/

#include <common.h>

#include <user.h>

#include <c_io.h>

#include <shell.h>

/*
** USER PROCESSES
**
** Each is designed to test some facility of the OS:
**
**	User(s)		Tests/Features
**	=======		===============================================
**	A, B, C		Basic operation
**	D		Spawns Z and exits
**	E, F, G		Sleep for different lengths of time
**	H		Doesn't call exit()
**	J		Tries to spawn 2*N_PCBS copies of Y
**	K		Spawns several copies of X, sleeping between
**	L		Spawns several copies of X, preempting between
**	M		Spawns W three times at low priority, reporting PIDs
**	N		Like M, but spawns X at high priority and W at low
**	P		Iterates three times, sleeping & printing system time
**	Q		Tries to execute a bogus system call (bad code)
**	R		Reading and writing three characters
**	S		Loops forever, sleeping 30 seconds at a time
**	T		Loops, fiddles with priority
**	U		Spawns W three times, sleeps, then kills them
**
**	W, X, Y, Z	Print characters (spawned by other processes)
**
** Output from user processes is always alphabetic.  Uppercase 
** characters are "expected" output; lowercase are "erroneous"
** output.
**
** More specific information about each user process can be found in
** the header comment for that function (below).
**
** To spawn a specific user process, uncomment its SPAWN_x
** definition in the user.h header file.
*/

/*
** Prototypes for all one-letter user main routines (even
** ones that may not exist, for completeness)
*/

void user_a( void ); void user_b( void ); void user_c( void );
void user_d( void ); void user_e( void ); void user_f( void );
void user_g( void ); void user_h( void ); void user_i( void );
void user_j( void ); void user_k( void ); void user_l( void );
void user_m( void ); void user_n( void ); void user_o( void );
void user_p( void ); void user_q( void ); void user_r( void );
void user_s( void ); void user_t( void ); void user_u( void );
void user_v( void ); void user_w( void ); void user_x( void );
void user_y( void ); void user_z( void );

/*
** Users A, B, and C are identical, except for the character they
** print out via write().  Each prints its ID, then loops 30
** times delaying and printing, before exiting.  They also verify
** the status return from write().
*/

void user_a( void ) {
	int i, j;
	ustatus_t status;

	c_puts( "User A running\n" );
	status = write( 'A' );
	if( status != U_SUCCESS ) {
		prt_ustat( "User A, write 1 status %s\n", status );
	}
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = write( 'A' );
		if( status != U_SUCCESS ) {
			prt_ustat( "User A, write 2 status %s\n", status );
		}
	}

	c_puts( "User A exiting\n" );
	exit( EXIT_SUCCESS );

	status = write( 'a' );	/* shouldn't happen! */
	if( status != U_SUCCESS ) {
		prt_ustat( "User A, write 3 status %s\n", status );
	}

}

void user_b( void ) {
	int i, j;
	ustatus_t status;

	c_puts( "User B running\n" );
	status = write( 'B' );
	if( status != U_SUCCESS ) {
		prt_ustat( "User B, write 1 status %s\n", status );
	}
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = write( 'B' );
		if( status != U_SUCCESS ) {
			prt_ustat( "User B, write 2 status %s\n", status );
		}
	}

	c_puts( "User B exiting\n" );
	exit( EXIT_SUCCESS );

	status = write( 'b' );	/* shouldn't happen! */
	if( status != U_SUCCESS ) {
		prt_ustat( "User B, write 3 status %s\n", status );
	}

}

void user_c( void ) {
	int i, j;
	ustatus_t status;

	c_puts( "User C running\n" );
	status = write( 'C' );
	if( status != U_SUCCESS ) {
		prt_ustat( "User C, write 1 status %s\n", status );
	}
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = write( 'C' );
		if( status != U_SUCCESS ) {
			prt_ustat( "User C, write 2 status %s\n", status );
		}
	}

	c_puts( "User C exiting\n" );
	exit( EXIT_SUCCESS );

	status = write( 'c' );	/* shouldn't happen! */
	if( status != U_SUCCESS ) {
		prt_ustat( "User C, write 3 status %s\n", status );
	}

}


/*
** User D spawns user Z.
*/

void user_d( void ) {
	ustatus_t status;
	int pid;

	c_puts( "User D running\n" );
	write( 'D' );

	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "User D fork() failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_z );
		prt_ustat( "User D, exec(Z) status %s\n", status );
		exit( EXIT_FAILURE );
	}

	write( 'D' );

	c_puts( "User D exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** Users E, F, and G test the sleep facility.
**
** User E sleeps for 10 seconds at a time.
*/

void user_e( void ) {
	int i;
	pid_t pid;

	pid = getpid();
	c_printf( "User E (%d) running\n", pid );
	write( 'E' );
	for( i = 0; i < 5 ; ++i ) {
		sleep( seconds_to_ms(10) );
		write( 'E' );
	}

	c_puts( "User E exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User F sleeps for 5 seconds at a time.
*/

void user_f( void ) {
	int i;
	pid_t pid;

	pid = getpid();
	c_printf( "User F (%d) running\n", pid );
	write( 'F' );
	for( i = 0; i < 5 ; ++i ) {
		sleep( seconds_to_ms(5) );
		write( 'F' );
	}

	c_puts( "User F exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User G sleeps for 15 seconds at a time.
*/

void user_g( void ) {
	int i;
	pid_t pid;

	pid = getpid();
	c_printf( "User G (%d) running\n", pid );
	write( 'G' );
	for( i = 0; i < 5; ++i ) {
		sleep( seconds_to_ms(15) );
		write( 'G' );
	}

	c_puts( "User G exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User H is like A-C except it only loops 5 times and doesn't
** call exit().
*/

void user_h( void ) {
	int i, j;

	c_puts( "User H running\n" );
	write( 'H' );
	for( i = 0; i < 5; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		write( 'H' );
	}

	sleep( seconds_to_ms(20) );
	c_puts( "User H returning without exiting!\n" );

}


/*
** User J tries to spawn 2*N_PCBS copies of user_y.
*/

void user_j( void ) {
	int i;
	int pid;
	ustatus_t status;

	c_puts( "User J running\n" );
	write( 'J' );

	for( i = 0; i < N_PCBS * 2 ; ++i ) {
		pid = fork( PRIO_STD );
		if( pid < 0 ) {
			prt_ustat( "User J fork() failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_y );
			prt_ustat( "User J exec(Y) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			write( 'J' );
		}
	}

	c_puts( "User J exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User K prints, goes into a loop which runs three times, and exits.
** In the loop, it does a spawn of user_x, sleeps 30 seconds, and prints.
*/

void user_k( void ) {
	int i;
	int pid;
	ustatus_t status;

	c_puts( "User K running\n" );
	write( 'K' );

	for( i = 0; i < 3 ; ++i ) {
		pid = fork( PRIO_STD );
		if( pid < 0 ) {
			prt_ustat( "User K fork() failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_x );
			prt_ustat( "User K exec(X) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			sleep( seconds_to_ms(30) );
			write( 'K' );
		}
	}

	c_puts( "User K exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User L is like user K, except that it prints times and doesn't sleep
** each time, it just preempts itself.
*/

void user_l( void ) {
	int i;
	int pid;
	time_t time;
	ustatus_t status;

	time = gettime();
	c_printf( "User L running, initial time %u\n", time );
	write( 'L' );

	for( i = 0; i < 3 ; ++i ) {
		time = gettime();
		pid = fork( PRIO_STD );
		if( pid < 0 ) {
			prt_ustat( "User L fork() failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_x );
			prt_ustat( "User L exec(X) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			sleep( 0 );
			write( 'L' );
		}
	}

	time = gettime();
	c_printf( "User L exiting at time %u\n", time );
	exit( EXIT_SUCCESS );

}


/*
** User M iterates spawns three copies of user W at low priority,
** reporting their PIDs.
*/

void user_m( void ) {
	int i;
	int pid;
	ustatus_t status;

	c_puts( "User M running\n" );
	for( i = 0; i < 3; ++i ) {
		pid = fork( PRIO_LOW );
		if( pid < 0 ) {
			prt_ustat( "User M fork() failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_w );
			prt_ustat( "User M exec(W) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			c_printf( "User M spawned W, PID %d\n", pid );
			write( 'M' );
		}
	}

	c_puts( "User M exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User N is like user M, except that it spawns user W at low priority
** and user Z at high priority.
*/

void user_n( void ) {
	int i;
	int pid;
	ustatus_t status;

	c_puts( "User N running\n" );
	for( i = 0; i < 3; ++i ) {
		pid = fork( PRIO_LOW );
		if( pid < 0 ) {
			prt_ustat( "User N fork(W) failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_w );
			prt_ustat( "User N exec(W) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			c_printf( "User N spawned W, PID %d\n", pid );
			write( 'N' );
		}

		pid = fork( PRIO_HIGH );
		if( pid < 0 ) {
			prt_ustat( "User N fork(Z) failed\n", U_FAILURE );
		} else if( pid == 0 ) {
			status = exec( user_z );
			prt_ustat( "User N exec(Z) status %s\n", status );
			exit( EXIT_FAILURE );
		} else {
			c_printf( "User N spawned Z, PID %d\n", pid );
			write( 'N' );
		}
	}

	c_puts( "User N exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User P iterates three times.  Each iteration sleeps for two seconds,
** then gets and prints the system time.
*/

void user_p( void ) {
	time_t time;
	int i;

	c_printf( "User P running, " );
	time = gettime();
	c_printf( " start at %08x\n", time );

	write( 'P' );

	for( i = 0; i < 3; ++i ) {
		sleep( seconds_to_ms(2) );
		time = gettime();
		c_printf( "User P reporting time %08x\n", time );
		write( 'P' );
	}

	c_printf( "User P exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User Q does a bogus system call
*/

void user_q( void ) {

	c_puts( "User Q running\n" );
	write( 'Q' );
	bogus();
	c_puts( "User Q returned from bogus syscall!?!?!\n" );
	exit( EXIT_SUCCESS );

}


/*
** User R loops 3 times reading/writing, then exits.
*/

void user_r( void ) {
	int i;
	char ch = '&';
	ustatus_t status;

	c_puts( "User R running\n" );
	sleep( seconds_to_ms(10) );
	for( i = 0; i < 3; ++i ) {
		do {
			write( 'R' );
			status = read( &ch );
			if( status != U_SUCCESS ) {
				prt_ustat( "User R, read status %s\n", status );
			} else if( ch == -1 ) {	/* wait a bit */
				sleep( seconds_to_ms(1) );
			}
		} while( ch == -1 );
		write( ch );
	}

	c_puts( "User R exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User S sleeps for 30 seconds at a time, and loops forever.
*/

void user_s( void ) {

	c_puts( "User S running\n" );
	write( 'S' );
	for(;;) {
		sleep( seconds_to_ms(30) );
		write( 'S' );
	}

	c_puts( "User S exiting!?!?!n" );
	exit( EXIT_SUCCESS );

}


/*
** User T changes its priority back and forth several times
*/

void user_t( void ) {
	prio_t prio1, prio2;
	int i, j;
	pid_t pid;
	ustatus_t status;

	pid = getpid();
	status = getprio( pid, &prio1 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, get prio status %s\n", status );
	} else {
		c_printf( "User T (%d) running, initial prio %d\n",
		  	  pid, prio1 );
	}
	
	write( 'T' );
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		write( 'T' );
	}

	status = setprio( pid, PRIO_HIGH );
	if( status != U_SUCCESS ) {
		write( 't' );
		prt_ustat( "User T, set prio #1 status %s\n", status );
	}
	status = getprio( pid, &prio2 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, get prio2 #1 status %s\n", status );
	} else {
		c_printf( "User T, prio was %d now %d\n", prio1, prio2 );
	}
	prio1 = prio2;
	
	write( 'T' );
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		write( 'T' );
	}

	status = setprio( pid, PRIO_LOW );
	if( status != U_SUCCESS ) {
		write( 't' );
		prt_ustat( "User T, set prio #2 status %s\n", status );
	}
	status = getprio( pid, &prio2 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, get prio2 #2 status %s\n", status );
	} else {
		c_printf( "User T, prio was %d now %d\n", prio1, prio2 );
	}
	prio1 = prio2;

	write( 'T' );
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		write( 'T' );
	}

	status = setprio( pid, PRIO_STD );
	if( status != U_SUCCESS ) {
		write( 't' );
		prt_ustat( "User T, set prio #3 status %s\n", status );
	}
	status = getprio( pid, &prio2 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, get prio2 #3 status %s\n", status );
	} else {
		c_printf( "User T, prio was %d now %d\n", prio1, prio2 );
	}
	prio1 = prio2;
	
	write( 'T' );
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		write( 'T' );
	}

	/*
	** Now, try to get the priority of a nonexistent PID
	*/

	status = getprio( 0, &prio2 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, getprio(0) status %s\n", status );
	} else {
		c_printf( "User T, getprio(0) returned %d\n", prio2 );
	}

	/*
	** Now, try to get the priority of the init process
	*/

	status = getprio( PID_INIT, &prio2 );
	if( status != U_SUCCESS ) {
		prt_ustat( "User T, getprio(INIT) status %s\n", status );
	} else {
		c_printf( "User T, getprio(INIT) returned %d\n", prio2 );
	}

	c_puts( "User T exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User U iterates three times, spawning copies of user W; then it
** sleeps for eight seconds, and then kills those processes.
*/

void user_u( void ) {
	int i;
	int pid[3];
	pid_t rpid;
	ustatus_t status;
	estatus_t rstat;

	c_puts( "User process U running!\n" );

	for( i = 0; i < 3; ++i ) {
		pid[i] = fork( PRIO_STD );
		if( pid[i] < 0 ) {
			write( 'u' );
		} else if( pid[i] == 0 ) {
			status = exec( user_w );
			if( status != U_SUCCESS ) {
				c_printf( "User U (%d )", getpid() );
				prt_ustat( " exec(W) status %s\n", status );
				exit( EXIT_FAILURE );
			}
		} else {
			c_printf( "User U spawned W, PID %d\n", pid[i] );
			write( 'U' );
		}
	}

	sleep( seconds_to_ms(8) );

	for( i = 0; i < 3; ++i ) {
		if( pid[i] > 0 ) {
			status = kill( pid[i] );
			c_printf( "User U kill PID %d", pid[i] );
			prt_ustat( " status %s\n", status );
		}
	}

	// collect exit status information

	do {
		status = wait( &rpid, &rstat );
		if( status == U_NOT_FOUND ) {
			break;
		}
		c_printf( "User U: child %d ", rpid );
		prt_estat( "exit status %s", rstat );
		if( status != U_SUCCESS ) {
			prt_ustat( " wait status %s", status );
		}
		c_putchar( '\n' );
	} while( 1 );

	exit( EXIT_SUCCESS );

}


/*
** User W prints W characters 20 times, sleeping 3 seconds between.
*/

void user_w( void ) {
	int i;
	pid_t pid;

	pid = getpid();
	c_printf( "User W (%d) running\n", pid );
	for( i = 0; i < 20 ; ++i ) {
		write( 'W' );
		sleep( seconds_to_ms(3) );
	}

	c_printf( "User W (%d) exiting\n", pid );
	exit( EXIT_SUCCESS );

}


/*
** User X prints X characters 20 times.  It is spawned multiple
** times, and prints its PID when started and exiting.
*/

void user_x( void ) {
	int i, j;
	pid_t pid;

	pid = getpid();
	c_printf( "User X (%d) running\n", pid );

	for( i = 0; i < 20 ; ++i ) {
		write( 'X' );
		for( j = 0; j < DELAY_STD; ++j )
			continue;
	}

	c_printf( "User X (%d) exiting\n", pid );
	exit( EXIT_SUCCESS );

}


/*
** User Y prints Y characters 10 times.
*/

void user_y( void ) {
	int i, j;

	c_puts( "User Y running\n" );
	for( i = 0; i < 10 ; ++i ) {
		write( 'Y' );
		for( j = 0; j < DELAY_ALT; ++j )
			continue;
		sleep( seconds_to_ms(1) );
	}

	c_puts( "User Y exiting\n" );
	exit( EXIT_SUCCESS );

}


/*
** User Z prints Z characters 10 times.
*/

void user_z( void ) {
	int i, j;
	pid_t pid, ppid;

	pid = getpid();
	ppid = getppid();

	c_printf( "User Z (%d) running, parent %d\n", pid, ppid );
	for( i = 0; i < 10 ; ++i ) {
		write( 'Z' );
		for( j = 0; j < DELAY_STD; ++j )
			continue;
	}

	ppid = getppid();
	c_printf( "User Z exiting, parent now %d\n", ppid );
	exit( EXIT_SUCCESS );

}


/*
** SYSTEM PROCESSES
*/

/*
** Idle process
*/

void idle( void ) {
	int i;
	pid_t pid;
	prio_t prio;
	ustatus_t status;

	pid = getpid();
	status = getprio( pid, &prio );
	if( status != U_SUCCESS ) {
		prt_ustat( "idle, getprio() status %s\n", status );
	}

	for(;;) {
		for( i = 0; i < DELAY_LONG; ++i )
			continue;
		//write( '.' );
	}


	exit( EXIT_FAILURE );
}


/*
** Initial process; it starts the other top-level user processes.
*/

void init( void ) {
	int pid;
	pid_t upid;
	ustatus_t status;
	estatus_t estat;

	c_puts( "Init started\n" );

	// Clear the screen
	write( '' );
	write( '$' );

	// always start the idle process first

	pid = fork( PRIO_DEFERRED );
	if( pid < 0 ) {
		prt_ustat( "init, fork() idle failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( idle );
		prt_ustat( "init, exec() idle, status %s\n", status );
		exit( EXIT_FAILURE );
	}

#ifdef SPAWN_A
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user A failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_a );
		prt_ustat( "init, exec() user A, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_B
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user B failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_b );
		prt_ustat( "init, exec() user B, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_C
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user C failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_c );
		prt_ustat( "init, exec() user C, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_D
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user D failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_d );
		prt_ustat( "init, exec() user D, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_E
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user E failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_e );
		prt_ustat( "init, exec() user E, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_F
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user F failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_f );
		prt_ustat( "init, exec() user F, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_G
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user G failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_g );
		prt_ustat( "init, exec() user G, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_H
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user H failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_h );
		prt_ustat( "init, exec() user H, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_J
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user J failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_j );
		prt_ustat( "init, exec() user J, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_K
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user K failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_k );
		prt_ustat( "init, exec() user K, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_L
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user L failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_l );
		prt_ustat( "init, exec() user L, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_M
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user M failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_m );
		prt_ustat( "init, exec() user M, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_N
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user N failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_n );
		prt_ustat( "init, exec() user N, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_P
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user P failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_p );
		prt_ustat( "init, exec() user P, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_Q
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user Q failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_q );
		prt_ustat( "init, exec() user Q, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_R
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user R failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_r );
		prt_ustat( "init, exec() user R, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_S
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user S failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_s );
		prt_ustat( "init, exec() user S, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_T
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user T failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_t );
		prt_ustat( "init, exec() user T, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_U
	pid = fork( PRIO_STD );
	if( pid < 0 ) {
		prt_ustat( "init, fork() user U failed\n", U_FAILURE );
	} else if( pid == 0 ) {
		status = exec( user_u );
		prt_ustat( "init, exec() user U, status %s\n", status );
		exit( EXIT_FAILURE );
	}
#endif

	write( '!' );

	/*
	** At this point, we go into an infinite loop waiting
	** for our children (direct, or inherited) to exit.
	*/

	pid = fork( PRIO_STD );
	if ( pid < 0 ) {
		// perror
	} else if ( pid == 0 ) {
		status = exec( run_shell );
		// should not return;
		exit( EXIT_FAILURE );
	}
	
	for(;;) {
		status = wait( &upid, &estat );
		if( status != U_SUCCESS ) {
			prt_ustat( "init, wait() status %s\n", status );
		} else {
			c_printf( "INIT: pid %d exited,", upid );
			prt_estat( " status %s\n", estat );
		}
	}

	/*
	** SHOULD NEVER REACH HERE
	*/

	c_printf( "*** INIT IS EXITING???\n" );
	exit( EXIT_FAILURE );

}
