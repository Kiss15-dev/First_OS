#include "idt.h"
#include "types.h"
#include "io.h"
#include "allocator.h"
#include "pmm.h"
#include "gdt.h"
#include "scheluder.h"

typedef struct
{
	uint64_t rax, rcx, rdx, rbx, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15;
} regs;

void kernel_main() {
	//add_page();
	init_gdt_kernel();
	pmm_init();
	idt_init();

	char* video_memory = (char*)0xB8000;
	video_memory[0] = 'Y';

	scheluder_start_first_proc();

	__asm__ volatile("sti");

	while(1);
}

void k_print() {
	char* video_memory = (char*)0xFFFFFFFF800B8000;
	video_memory[0] = 'Y';
	video_memory[2] = 'R';
	video_memory[4] = 'A';
}

void syscall_handler(regs* reg) {
	switch(reg->rax) {
		case 1:
			k_print();
			break;
		default:
			reg->rax = -1;
			break;
	}
}

void __stack_chk_fail(void) {
    while(1) {
        __asm__ __volatile__("hlt"); 
    }
}