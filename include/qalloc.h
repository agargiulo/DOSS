/*
**
** File:	qalloc.h
**
** Author:	4003-506 class of 20123
**
** Contributor: Owen Royall-Kahin
**
** Description:	Quick memory allocation header
*/

#ifndef _QALLOC_H_
#define _QALLOC_H_

#define __SP2_KERNEL__

#include <common.h>


/*
** Prototypes
*/

void * qalloc( size_t size );

// No qfree: I hope you have a lot of memory ;)

#endif
