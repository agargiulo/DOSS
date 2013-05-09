/*
** File: shell/nettest.S
**
** Author: Anthony Gargiulo
**
** Description: Test the network card
*/

#define    __SP2_KERNEL__
#define    __SP2_ASM__

#include <common.h>
#include <8255x.h>

.global CSR_BAR
#.global run_nettest

fmt:	.string "COMMAND: 0x%08x\n"

nic_wait:
	/* enter $0, $0 */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx

	/* uint16_t timeout = 10000; */
	/* movw	$10000, %cx */
	/* uint8_t i = 0; */
	xorb	%bl, %bl

nic_wait_while:
	/* __inb(CSR_BAR + E_CSR_SCB_COM_WORD) */
	xorl	%eax, %eax
	movl	CSR_BAR, %edx
	addl	E_CSR_SCB_COM_WORD, %edx
	inb		(%dx)
	/* end __inb(CSR_BAR + E_CSR_SCB_COM_WORD) */
	
	/* __inb(...) != 0 */
	cmpb	$0, %al
	je		nic_wait_end_while
	
	/* --timeout != 0 */
	/*
	decw	%cx
	cmpw	$0, %cx
	je		nic_wait_end_while
	*/

	/* At this point, the loop conditions have been met */
	/* and dealt with, moving on to the body :D */
nic_wait_while_body:
	cmpb	$0, %bl
	jne		nic_wait_noprint
	pushl	%ecx
	pushl	$0x2E	/* 0x2E == 46 == ascii for '.' */
	call	c_putchar
	addl	$4, %esp
	popl	%ecx
nic_wait_noprint:
	incb	%bl
	movzbw	%bl, %ax
	movb	$100, %dl
	divb	%dl
	movb	%ah, %bl
	pushl	%ecx
	pushl	$1000
	call	 __delay
	addl	$4, %esp
	popl	%ecx
	jmp		nic_wait_while

nic_wait_end_while:
	pushl	%ecx
	pushl	$0x0A	/* 0x0A == 10 == ascii for '\n' */
	call	c_putchar
	addl	$4, %esp
	popl	%ecx
nic_wait_ret:
	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret
	
run_nettest: 
	/* enter $0, $0 */
	pushl	%ebp
	movl	%esp, %ebp

	/* __outl(CSR_BAR + E_CSR_PORT, 0x00000000); */
	movl	$0, %eax
	movl	CSR_BAR, %edx
	addl	E_CSR_PORT, %edx
	outl	(%dx)
	/* end __outl(CSR_BAR + E_CSR_PORT, 0x00000000); */

	/* __delay(10000); */
	pushl	$10000
	call	__delay
	addl	$4, %esp
	/* end __delay(100); */
	
	/* __outl(CSR_BAR + E_CSR_SCB_GEN_PTR, 0x00000000); */
	movl	$0, %eax
	movl	CSR_BAR, %edx
	addl	E_CSR_SCB_GEN_PTR, %edx
	outl	(%dx)
	/* end __outl(CSR_BAR + E_CSR_SCB_GEN_PTR, 0x00000000); */

	/* __outb(CSR_BAR + E_CSR_SCB_COM_WORD, 0x60); */
	movl	$0x60, %eax

	/* print out the command word (just checking my syntax here) */
	pushl	%eax
	pushl	$fmt
	call	c_printf
	addl	$8, %esp
	/* end print  */

	movl	CSR_BAR, %edx
	addl	E_CSR_SCB_COM_WORD, %edx
	outb	(%dx)
	/* end __outb(CSR_BAR + E_CSR_SCB_COM_WORD, 0x60); */

	call nic_wait

run_nettest_ret:
	/* leave */
	movl	%ebp, %esp
	popl	%ebp
	ret
