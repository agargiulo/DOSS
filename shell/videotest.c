#define __SP2_KERNEL__

/*
** File: shell/videotest.c
**
** Author: Alex Walcutt
**
** Description: videotest command
*/

#include <common.h>
#include <video.h>

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

void run_videotest( int argc, char **argv )
{
	_video_test();
}
