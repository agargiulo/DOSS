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
void run_echo(int argc, char **argv);
void run_lspci(int argc, char **argv);
void run_test(int argc, char **argv);
void run_halt(int argc, char **argv);

void run_lsblk(int argc, char **argv);
void run_ls(int argc, char **argv);
void run_cat(int argc, char **argv);
void run_df(int argc, char **argv);
void run_touch(int argc, char **argv);
void run_append(int argc, char **argv);
void run_stat(int argc, char **argv);
void run_mount(int argc, char **argv);
void run_systime(int argc, char **argv);
void run_rm(int argc, char **argv);


// Write a message to both the serial terminal and the console
void wall_s(char *message);
void wall_ch(char ch);
/*
** Prototypes
*/

void run_shell( void );

#endif

#endif
