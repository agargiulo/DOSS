/*
** File:	ofs.h
**
** Author:	4003-506 class of 20123
**
** Contributor: Owen Royall-Kahin
**
** Description:	Header for Owen's file system
*/

#ifndef _OFS_H_
#define _OFS_H_

#include <common.h>

/*
** Types
*/

typedef struct file {
	char		*name;
	uint32_t	size;
	uint32_t	cluster;
	uint8_t		id;
} file_t;

/*
** Prototypes
*/

void _fs_init( void );

/**
 * fcreat
 * Creates and opens a file, returning the pointer to the new file_t.
 */
file_t * fcreat( char * name );

/**
 * fopen
 * Opens the file with the given filename, returns the pointer to the file_t.
 */
file_t * fopen( char * name );

/**
 * fclose
 * Closes the file, returns 0 on success.
 */
int fclose( file_t * file );

/**
 * fseek
 * Currently not supported.
 */
// int fseek( file_t * file, uint32_t offset );

/**
 * fread
 * Read size blocks count times into buffer, from source file.
 */
size_t fread( void * buffer, size_t size, size_t count, file_t * file );

/**
 * fwrite
 * Writes size blocks count times, into destination file from source buffer.
 */ 
size_t fwrite( const void * buffer, size_t size, size_t count, file_t * file );

// Other FS functions
void print_fat( int n );
void print_dt( int n );
void df( void );
void stat( char * filename );
void ls( void );


#endif
