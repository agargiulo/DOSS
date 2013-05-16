/*
** SCCS ID:	%W%	%G%
**
** File:	video.c
**
** Author:	Alex Walcutt
**
** Contributor: 
**
** Description:	Contains methods for updating the video display
*/

#define __SP2_KERNEL__

#include <common.h>
#include <video_s.h>
#include <video.h>
#include <vga.h>
#include <startup.h>
#include <draw.h>

#define MAX_OBJECTS 10

byte_t vbuf[64000] = {0};
video_object objects[MAX_OBJECTS];
int nextObjectID = 0;

void _video_init( void )
{
	pid_t pid = fork( PRIO_HIGH );
	if( pid == 0 )
	{
		exec(_video_sync);
	}
}

void _video_test( void )
{
	_video_setmode_13();
	_video_clear();
	_video_init();

	point topleft = {0,0};
	point botright = {10, 10};
	Color c = BLUE;
	video_object *test = _video_obj_create(topleft, botright, c);
		
	int ch = c_getchar();
	switch(ch)
	{
		case 115:
			_video_obj_move(test, 0, 2);
			break;
		case 97:
			_video_obj_move(test, -2, 0);
			break;
		case 119:
			_video_obj_move(test, 0, -2);
			break;
		case 100:
			_video_obj_move(test, 2, 0);
			break;
		case 99:
			_video_obj_recolor(test, test->color + 1);
			break;
	}
}

void _video_clear( void )
{
	for(int i = 0; i < 320*200; i++) vbuf[i] = 0;
	_video_redraw();
}

void _video_redraw( void )
{
	unsigned char *vmem = (unsigned char*) 0xa0000;
	for(int i = 0; i < 64000; i++) vmem[i] = vbuf[i];
}

void _video_sync( void )
{
	for(;;)
	{
		for(int i = 0; i < nextObjectID; i++)
		{
			video_object obj = objects[i];
			point oldp1 = obj.oldp1;
			point oldp2 = obj.oldp2;
			point p1 = obj.p1;
			point p2 = obj.p2;
			Color c = obj.color;
			_draw_rect_filled(&oldp1, &oldp2, 0);
			_draw_rect_filled(&p1, &p2, c);
		}

		_video_redraw();
		sleep(0);
	}
}

video_object* _video_obj_create(point p1, point p2, Color c)
{
	if (nextObjectID >= MAX_OBJECTS) return;

	video_object newObj;
	newObj.id = nextObjectID;
	newObj.p1 = p1;
	newObj.p2 = p2;
	newObj.color = c;

	objects[nextObjID++] = newObj;
	
	return &newObj;
}

void _video_obj_move(video_object *obj, int x, int y)
{
	obj->oldp1 = obj->p1;
	obj->oldp2 = obj->p2;

	obj->p1.x += x; obj->p2.x += x;
	obj->p1.y += y; obj->p2.y += y;

	if(obj->p1.x < 0) {obj->p1.x += 320; obj->p2.x += 320;}
	if(obj->p1.y < 0) {obj->p1.y += 200; obj->p2.y += 200;}
	if(obj->p2.x >= 320) {obj->p1.x -= 320; obj->p2.x -= 320;}
	if(obj->p2.y >= 200) {obj->p1.y -= 200; obj->p2.y -= 200;}
}

void _video_obj_recolor(video_object *obj, enum Color color)
{
	obj->color = color;
}
