/*
** File: include/net_handler.h
**
** Author: Anthony Gargiulo
**
** Description: 
*/

#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

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

/*
** Prototypes
*/

void _net_handler (int vector, int code);

#endif

#endif
