[bits 64]
%include "macros.inc"

global _start
global keyboard_isr
global timer_isr
global start_first_process
global user_test

extern kernel_main
extern keyboard_handler
extern scheluder

_start:
	mov rax, 0xFFFFFFFF80001000
	mov qword [rax], 0

	mov rax, cr3
	mov cr3, rax

	call kernel_main
	jmp $

keyboard_isr:
	pushall
	call keyboard_handler
	popall
	iretq

timer_isr:
	pushall
	call scheluder
	popall
	iretq

start_first_process:
	mov cr3, rsi
	mov rsp, rdi
	popall
	iretq

user_test:
	jmp user_test