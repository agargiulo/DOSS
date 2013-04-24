/*
** SCCS ID:	%W%	%G%
**
** File:	?
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
*/

#define	__SP2_KERNEL__

#include <string.h>

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

int memcmp ( const void * ptr1, const void * ptr2, size_t num ) {
	
	int i;
	
	for ( i = 0; i != num; ++i ) {
		if ( ((uint8_t*)ptr1)[i] == ((uint8_t*)ptr2)[i] ) continue;
		return ((uint8_t*)ptr1)[i] - ((uint8_t*)ptr2)[i];
	}
	
	return 0;
}

void * memcpy ( void * destination, const void * source, size_t num ) {
	int i;
	
	for ( i = 0; i != num; ++i ) {
		((uint8_t*)destination)[i] = ((uint8_t*)source)[i];
	}
	
	return destination;
}

void * memset ( void * ptr, int value, size_t num ) {
	int i;
	
	for ( i = 0; i != num; ++i ) {
		((int*)ptr)[i] = value;
	}
	
	return ptr;
}

size_t strlen ( const char * str ) {
	int i;
	
	for ( i = 0; str[i] != 0; ++i);
	
	return i;
}

int strcmp ( const char * str1, const char * str2 ) {
	int i;
	
	for ( i = 0;; ++i ) {
		if ( str1[i] == 0 || str2[i] == 0 ) break;
		if ( str1[i] != str2[i] ) break;
	}
	
	return str1[i] - str2[i];
}

char * strcpy ( char * destination, const char * source ) {
	int i;
	
	for ( i = 0; source[i] != 0 && destination[i] != 0; ++i) {
		destination[i] = source[i];
	}
	
	return destination;
}

char * strncpy ( char * destination, const char * source, size_t num ) {
	int i;
	
	for ( i = 0; i != num; ++i ) {
		destination[i] = source[i];
	}
	
	return destination;
}

#define isspace(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\r')
#define isdigit(ch) ((ch) >= '0' && (ch) <= '9')
int atoi ( char * string ) {
	int num = 0;
	int negative = 0;
	while (isspace(*string))
	{
		string++;
	}
	if (*string == '-')
	{
		negative = 1;
		string++;
	}
	else if (!isdigit(*string))
	{
		num = -1;
	}
	while (isdigit(*string))
	{
		num =  (num * 10) + (unsigned int) (*(string++) - '0');
	}
	if (negative)
	{
		num *= -1;
	}
	return num;
}
