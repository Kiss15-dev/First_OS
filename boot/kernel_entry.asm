[bits 64]
%include "macros.inc"

global _start
global keyboard_isr
global timer_isr
global syscall_isr
global user_test
global user_test_end
global start_first_process

extern kernel_main
extern keyboard_handler
extern scheluder
extern syscall_handler

section .text
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

syscall_isr:
	pushall

	mov rdi, rsp
	call syscall_handler

	popall
	iretq

start_first_process:
	mov cr3, rsi
	mov rsp, rdi
	popall
	iretq

user_test:
	jmp user_test
user_test_end: