
.text

.globl _video_set_mode_13
.globl _video_set_mode_text
.globl _video_plot_pixel_test

_video_set_mode_13:
	xorl %eax,  %eax
	mov  $0x13, %al
	int  $0x10
	ret

_video_set_mode_text:
	xorl %eax,  %eax
	mov  $0x03, %al
	int  $0x10
	ret

_video_plot_pixel_test:
	xorl %eax,  %eax
	xorl %ecx,  %ecx
	xorl %edx,  %edx
	mov  $0x0C, %ah
	mov  $0x02, %al
	mov  $10,   %cx
	mov  $10,   %dx
	int  $0x10
	ret
