
#include <common.h>
#include <video_s.h>

void _video_test( void )
{
	unsigned char *pixel = 0xa0000;
	for(int i = 0; i < 32000; i++)
	{
		pixel[i] = 13;
	}
}
