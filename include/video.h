/*
** SCCS ID:	%W%	%G%
**
** File:	video.h
**
** Author:	Alex Walcutt
**
** Contributor:
**
** Description:	Prototypes for video rendering methods
*/

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

#include <queue.h>

/*
** Types
*/

typedef struct video_object
{
	int id;
	point p1, p2, oldp1, oldp2;
	enum Color color;
} video_object;

/*
** Globals
*/

/*
** Prototypes
*/

void _video_init( void );
void _video_test( void );

void _video_clear( void );
void _video_redraw( void );
void _video_sync( void );

void _video_obj_move(video_object *obj, int x, int y);
void _video_obj_recolor(video_object *obj, enum Color color);

#endif

#endif
