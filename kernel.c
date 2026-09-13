#include "idt.h"
#include "types.h"
#include "io.h"
#include "allocator.h"

volatile uint64_t timer_tick = 0;

void timer_handler() {
	timer_tick++;

	outb(0x20, 0x20);
}

void kernel_main() {
	add_page();
	idt_init();

	int* a = (int*)kmalloc(4);

	__asm__ volatile("sti");

	while(1);
}

void __stack_chk_fail(void) {
    while(1) {
        __asm__ __volatile__("hlt"); 
    }
}