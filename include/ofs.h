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
	char		name[112];
	uint32_t	start_cluster;
} file_t;

/*
** Prototypes
*/

void _fs_init( void );

file_t * fopen( char * name );

int fclose( file_t * file );

int fseek( file_t * file, uint32_t offset );

size_t fread( void * buffer, size_t size, size_t count, file_t * file );

size_t fwrite( const void * buffer, size_t size, size_t count, file_t * file );


#endif
