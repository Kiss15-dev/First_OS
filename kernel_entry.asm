[bits 32]

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
	pushad
	call keyboard_handler
	popad
	iretd

timer_isr:
	pushad
	call timer_handler
	popad
	iretd