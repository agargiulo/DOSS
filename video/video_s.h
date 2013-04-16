/*
** SCCS ID:	%W%	%G%
**
** File:	video_s.h
**
** Author:	Alex Walcutt
**
** Contributor:
**
** Description:	Assembly language functions for common VESA functions
*/

#ifndef _VIDEO_S_H_
#define _VIDEO_S_H_

#include <common.h>

/*
** General (C and/or assembly) definitions
*/



void _video_set_mode_13( void );
void _video_plot_pixel_test( void );
void _video_set_mode_text( void );


#ifndef __SP2_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

#endif

#endif
