/*
** SCCS ID:	%W%	%G%
**
** File:	shell.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	?
*/

#ifndef _SHELL_H_
#define _SHELL_H_

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

/*
** Globals
*/
void run_clear(int argc, char **argv);
void run_help(int argc, char **argv);
void run_ps(int argc, char **argv);
void run_reboot(int argc, char **argv);

// Write a message to both the serial terminal and the console
void wall_s(char *message);
void wall_ch(char ch);
/*
** Prototypes
*/

void run_shell( void );

#endif

#endif
