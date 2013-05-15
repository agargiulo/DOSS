#define __SP2_KERNEL__

#include <common.h>
#include <video_s.h>
#include <video.h>
#include <vga.h>
#include <startup.h>

byte_t vbuf[64000] = {0};

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
	unsigned int i;
	unsigned char *pixel = (unsigned char*) 0xa0000;

	//Switch to Mode 0x13
	__delay(10);
	_video_setmode_13();
	_video_init();

	for(i = 0; i < 320*200; i++) vbuf[i] = 0;

	enum Color c;

	//_video_point test
	c = LIGHTBLUE;
	_video_point(80, 80, c);

	//_video_line test
	point p1 = {20,20};
	point p2 = {320,120};
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

	__delay(10);
	_video_redraw();
	for(i=0; i < 320*200; i++) vbuf[i] = 0;
	__delay(10);
	_video_redraw();
	
	c = WHITE;
	_video_box_filled(&p5, &p6, c);
	__delay(10);
	_video_redraw();

	//Switch back to Text Mode
	//__delay(10);
	//for(i = 0; i < 320*200; i++) pixel[i] = 0;
	//__delay(10);
	//_video_setmode_text();

}

void _video_test2( void )
{
	_video_setmode_13();
	_video_clear();
	point p1 = {0,0};
	point p2 = {10,10};
	point oldp1 = p1;
	point oldp2 = p2;
	Color c = WHITE;
	//demobox = {p1, p2, oldp1, oldp2, c, 0};

	for(;;)
	{
		_video_box_filled(&oldp1, &oldp2, 0);
		_video_box_filled(&p1, &p2, c);
		_video_redraw();
		sleep(0);
		oldp1 = p1; oldp2 = p2;
		
		int ch = c_getchar();
		switch(ch)
		{
			case 115:
				p1.y++; p2.y++;
				break;
			case 97:
				p1.x--; p2.y--;
				break;
			case 119:
				p1.y--; p2.y--;
				break;
			case 100:
				p1.x++; p2.x++;
				break;
		}

		if(p1.x < 0) {p1.x = 309; p2.x = 319;}
		if(p1.y < 0) {p1.y = 189; p2.y = 199;}
		if(p2.x >= 320) {p1.x = 0; p2.x = 10;}
		if(p2.y >= 200) {p1.y = 0; p2.y = 10;}

	}
}

void _video_point(int x, int y, enum Color c)
{
	unsigned char *vmem = (unsigned char*) 0xa0000;

	int offset = (y * 320) + x;
	//vmem[offset] = (int) c;
	vbuf[offset] = (int) c;
}

int abs(int num)
{
	if (num < 0) return num * -1;
	else return num;
}

void _video_clear( void )
{
	for(int i = 0; i < 320*200; i++) vbuf[i] = 0;
	_video_redraw();
}

//Line algorithm courtesy Owen Royall-Kahin
void _video_line(point *p1, point *p2, enum Color c)
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
			_video_point(y, x, c);
		}
		else
		{
			_video_point(x, y, c);
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

void _video_redraw()
{
	unsigned char *vmem = (unsigned char*) 0xa0000;
	for(int i = 0; i < 64000; i++) vmem[i] = vbuf[i];
}

void _video_sync()
{
	for(;;)
	{
		//_video_redraw();

		sleep(99999);
	}
}

void _video_obj_move(video_object *obj, point *topleft)
{
	int horiz = (obj->p2.x - obj->p1.x);
	int vert = (obj->p2.y - obj->p1.y);
	
	point newbotright = {topleft->x + horiz, topleft->y + vert};

	obj->oldp1 = obj->p1;
	obj->oldp2 = obj->p2;
	obj->p1 = *topleft;
	obj->p2 = newbotright;
}

void _video_obj_resize(video_object *obj, int dx, int dy)
{
	obj->oldp1 = obj->p1;
	obj->oldp2 = obj->p2;

	obj->p2.x += dx;
	obj->p2.y += dy;
}

void _video_obj_recolor(video_object *obj, enum Color color)
{
	obj->color = color;
}

void _video_obj_erase(video_object *obj)
{
	//add to refresh queue I guess
}
