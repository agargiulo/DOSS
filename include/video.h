/*
** SCCS ID:	%W%	%G%
**
** File:	video.h
**
** Author:	Alex Walcutt
**
** Contributor:
**
** Description:	Prototypes for common mode 13h video functions
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

/*
** Types
*/

typedef enum Color
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
} Color;

typedef struct point {int x,y;} point;

/*
** Globals
*/

/*
** Prototypes
*/

int abs(int num);
void _video_test( void );
void _video_point(int x, int y, enum Color c);
void _video_line(point *p1, point *p2, enum Color c);
void _video_box(point *p1, point *p2, enum Color c);
void _video_box_filled(point *p1, point *p2, enum Color c);

#endif

#endif
