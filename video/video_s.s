
.text

.globl _video_set_mode_13
.globl _video_set_mode_text
.globl _video_plot_pixel_test

_video_set_mode_13:
	xorl %eax, %eax
	mov  %al, 0x13
	int $0x10
	ret

_video_set_mode_text:
	xorl %eax, %eax
	mov %al, 0x03
	int $0x10
	ret

_video_plot_pixel_test:
	xorl %eax, %eax
	xorl %ecx, %ecx
	xorl %edx, %edx
	mov  %ah, 0x0C
	mov  %al, 0x02
	mov  %cx, 10
	mov  %dx, 10
	int $0x10
	ret
