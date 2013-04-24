/*
** SCCS ID:	%W%	%G%
**
** File:	string.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
*/

#ifndef _STRING_H_
#define _STRING_H_

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

/*
** Globals
*/

/*
** Prototypes
*/

int memcmp ( const void * ptr1, const void * ptr2, size_t num );

void * memcpy ( void * destination, const void * source, size_t num );

void * memset ( void * ptr, int value, size_t num );

size_t strlen ( const char * str );

int strcmp ( const char * str1, const char * str2 );

char * strcpy ( char * destination, const char * source );

char * strncpy ( char * destination, const char * source, size_t num );

int atoi ( char * nptr );

#endif

#endif
