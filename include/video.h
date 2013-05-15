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

typedef struct video_object
{
	point p1, p2, oldp1, oldp2,
	enum Color color,
	uint8_t flags;
} video_object;

/*
** Globals
*/

extern queue_t video_refresh;
extern byte_t vbuf[64000] = {0};

/*
** Prototypes
*/

int abs(int num);

void _video_test( void );

void _video_point(int x, int y, enum Color c);
void _video_line(point *p1, point *p2, enum Color c);
void _video_box(point *p1, point *p2, enum Color c);
void _video_box_filled(point *p1, point *p2, enum Color c);

void _video_sync();

void _video_obj_move(video_object *obj, point *topleft);
void _video_obj_resize(video_object *obj, point *botright);
void _video_obj_recolor(video_object *obj, enum Color color);
void _video_obj_erase(video_object *obj);

#endif

#endif
