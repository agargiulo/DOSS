
#include <common.h>
#include <video_s.h>

void _video_test( void )
{
	c_puts("Entering Video Test...\n");
	_video_set_mode_13();
	c_puts("Video Set Mode Complete\n");
	_video_plot_pixel_test();
	c_puts("Plot Pixel Test Complete\n");
}
