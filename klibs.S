/*
** sccs id:	@(#)klibs.S	1.1	4/2/13
**
** File:	klibs.S
**
** Author:	4003-506 class of 20123
**
** Contributor:
**
** Description:	Kernel library assembly-language implementations
*/

#define	__SP2_KERNEL__
#define	__SP2_ASM__

/*
** _get_ebp - return current contents of EBP at the time of the call
**
** Could be used, e.g., by _kpanic to print a traceback
*/

	.globl	_get_ebp
_get_ebp:
	movl	%ebp, %eax
	ret
