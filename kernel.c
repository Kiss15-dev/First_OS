#include "idt.h"

void kernel_main() {
	idt_init();

	//__asm__ volatile("sti");

	while(1);
}