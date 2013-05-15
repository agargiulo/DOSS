/*
**
** File:	qalloc.c
**
** Author:	4003-506 class of 20123
**
** Contributor: Owen Royall-Kahin
**
** Description:	Quick allocation implementation
*/

#define	__SP2_KERNEL__
#include <common.h>
#include <qalloc.h>

/*
** PRIVATE DEFINITIONS
*/

#define	ARB_LOCATION	0x3FFFFFFF

/*
** PRIVATE GLOBAL VARIABLES
*/

uint8_t *base_ptr = (uint8_t*)ARB_LOCATION;

/*
** PUBLIC FUNCTIONS
*/

void * qalloc( size_t size ) {
	uint8_t *result = base_ptr;
	base_ptr += size;
	return (void*)result;
}