
#include <common.h>
#include <video_s.h>
#include <video.h>

void _video_test( void )
{
	//_video_point test
	/*for(int y = 0; y < 100; y++)
	{
		for(int x = 0; x < 320; x++)
		{
			_video_point(x, y);
		}
	}*/

	enum Color c;

	//_video_line test
	point p1 = {20,20};
	point p2 = {20,120};
	c = LIGHTMAGENTA;
	_video_line(&p1, &p2, c);

	//_video_box test
	point p3 = {30,30};
	point p4 = {40,40};
	c = LIGHTCYAN;
	_video_box(&p3, &p4, c);

	//_video_box_filled test
	point p5 = {150,50};
	point p6 = {300,150};
	c = YELLOW;
	_video_box_filled(&p5, &p6, c);
}

void _video_point(int x, int y, enum Color c)
{
	int offset = (y * 320) + x;
	unsigned char *pixel = (unsigned char*) 0xa0000;
	pixel[offset] = (int) c;
}

void _video_line(point *p1, point *p2, enum Color c)
{
	point top,bot;
	double deltaX,x;
	int i,ix;

	if( p2->y > p1->y )
	{
		top = *p1;
		bot = *p2;
	}
	else
	{
		top = *p2;
		bot = *p1;
	}

	if( top.y == bot.y )
	{
		if( top.x < bot.x )
		{
			for(i = top.x; i <= bot.x; i++)
			{
				_video_point(i, top.y, c);
			}
		}
		else
		{
			for(i = bot.x; i <= top.x; i++)
			{
				_video_point(i, top.y, c);
			}
		}
		
		return;
	}

	deltaX = (bot.x - top.x) / (bot.y - top.y);
	x = top.x;

	for(i = top.y; i <= bot.y; i++)
	{
		ix = x;
		_video_point(ix, i, c);
		x += deltaX;
	}

	return;
}

void _video_box(point *p1, point *p2, enum Color c)
{
	point corner1 = {p1->x, p1->y};
	point corner2 = {p2->x, p1->y};
	point corner3 = {p2->x, p2->y};
	point corner4 = {p1->x, p2->y};

	_video_line(&corner1, &corner2, c);
	_video_line(&corner2, &corner3, c);
	_video_line(&corner3, &corner4, c);
	_video_line(&corner4, &corner1, c);
}

void _video_box_filled(point *p1, point *p2, enum Color c)
{
	for(int x = p1->x; x <= p2->x; x++) 
	{
		point start = {x, p1->y};
		point end = {x, p2->y};
		_video_line(&start, &end, c);
	}
}

