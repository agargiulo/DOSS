/*
** SCCS ID:	@(#)types.h	1.1	4/2/13
**
** File:	types.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	General data type declarations
*/

#ifndef _TYPES_H_
#define _TYPES_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

// generic integer types

typedef unsigned int	uint_t;

// size-specific integer types

typedef char		int8_t;
typedef unsigned char	uint8_t;
typedef int8_t		byte_t;
typedef uint8_t		ubyte_t;

typedef short		int16_t;
typedef unsigned short	uint16_t;
typedef int16_t		short_t;
typedef uint16_t	ushort_t;

typedef long		int32_t;
typedef unsigned long	uint32_t;
typedef uint32_t	ulong_t;

typedef long long	uint64_t;

#ifndef MIXED_INCLUDES
typedef uint32_t	size_t;
#endif

// system call status return

typedef uint8_t		ustatus_t;

// user-visible process-related types

typedef	uint16_t	pid_t;		// process id
typedef uint8_t		prio_t;		// process priority
typedef uint8_t		estatus_t;	// exit status

// system time

typedef uint32_t	time_t;

#ifdef __SP2_KERNEL__

/*
** OS-only type declarations
*/

// status return value

typedef	uint8_t		status_t;

// sorting key

typedef union key {
	int32_t  i;
	uint32_t u;
	time_t   t;
	void    *v;
} key_t;

#endif

/*
** Globals
*/

/*
** Prototypes
*/

#endif

#endif
