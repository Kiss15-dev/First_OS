[bits 64]
%include "macros.inc"

global _start
global keyboard_isr
global timer_isr

extern kernel_main
extern keyboard_handler
extern timer_handler

_start:
	call kernel_main
	jmp $

keyboard_isr:
	pushall
	call keyboard_handler
	popall
	iretq

timer_isr:
	pushall
	call timer_handler
	popall
	iretq