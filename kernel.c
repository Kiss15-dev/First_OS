#include "idt.h"
#include "types.h"
#include "io.h"

volatile uint32_t timer_tick = 0;

void timer_handler() {
	timer_tick++;

	outb(0x20, 0x20);
}

void kernel_main() {
	idt_init();

	__asm__ volatile("sti");

	while(1);
}