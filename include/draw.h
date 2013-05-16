/*
** SCCS ID:	%W%	%G%
**
** File:	draw.h
**
** Author:	Alex Walcutt
**
** Contributor:
**
** Description:	Prototype methods for basic drawing
*/

#ifndef _DRAW_H_
#define _DRAW_H_

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

_draw_point(int x, int y, Color c);
_draw_line(point *p1, point *p2, Color c);
_draw_rect(point *p1, point *p2, Color c);
_draw_rect_filled(point *p1, point *p2, Color c);

#endif

#endif
