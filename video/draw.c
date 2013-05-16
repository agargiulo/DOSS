/*
** SCCS ID:	%W%	%G%
**
** File:	draw.c
**
** Author:	Alex Walcutt
**
** Contributor: Owen Royall-Kahn
**
** Description:	Contains methods for drawing points, lines, and rectangles
*/

#define	__SP2_KERNEL__

#include <common.h>
#include <drawing.h>

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

int abs(int num)
{
	if (num < 0) return num * -1;
	else return num;
}

_draw_point(int x, int y, Color c)
{
	int offset = (y * 320) + x;
	vbuf[offset] = (int) c;
}

_draw_line(point *p1, point *p2, Color c)
{
	int dE, dNE, x, y, d;

	int steep = abs(p2->y - p1->y) > abs(p2->x - p1->x) ? 1 : 0;
	if (steep == 1)
	{
		int temp;

		temp = p1->x;
		p1->x = p1->y;
		p1->y = temp;

		temp = p2->x;
		p2->x = p2->y;
		p2->y = temp;
	}

	int dy = abs(p2->y - p1->y);
	int dx = abs(p2->x - p1->x);

	dE = 2 * dy;
	dNE = 2 * (dy - dx);
	d = dE - dx;

	int xinc = p1->x < p2->x ? 1 : -1;
	int yinc = p1->y < p2->y ? 1 : -1;

	for(x = p1->x, y = p1->y; x != p2->x + xinc; x += xinc)
	{
		if (steep == 1)
		{
			_draw_point(y, x, c);
		}
		else
		{
			_draw_point(x, y, c);
		}

		if (d <= 0)
		{
			d += dE;
		}
		else
		{
			y += yinc;
			d += dNE;
		}
	}

}

_draw_rect(point *p1, point *p2, Color c)
{
	point corner1 = {p1->x, p1->y};
	point corner2 = {p2->x, p1->y};
	point corner3 = {p2->x, p2->y};
	point corner4 = {p1->x, p2->y};

	_draw_line(&corner1, &corner2, c);
	_draw_line(&corner2, &corner3, c);
	_draw_line(&corner3, &corner4, c);
	_draw_line(&corner4, &corner1, c);
}

_draw_rect_filled(point *p1, point *p2, Color c)
{
	for(int x = p1->x; x <= p2->x; x++) 
	{
		point start = {x, p1->y};
		point end = {x, p2->y};
		_draw_line(&start, &end, c);
	}
}
