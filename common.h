/*
** SCCS ID:	@(#)common.h	1.1	4/2/13
**
** File:	common.h
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Standard includes needed in all C source files
*/

#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef __SP2_ASM__

// only pull these in if we're not in assembly language

#include "defs.h"
#include "types.h"

#include "c_io.h"
#include "support.h"

#ifdef __SP2_KERNEL__

// OS needs the kernel library headers and the system headers

#include "system.h"
#include "klib.h"

#else

// User code nees onlyl the user library headers

#include "ulib.h"

#endif

#endif

#endif
