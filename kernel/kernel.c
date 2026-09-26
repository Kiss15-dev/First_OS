#include "idt.h"
#include "types.h"
#include "io.h"
#include "allocator.h"
#include "pmm.h"
#include "gdt.h"
#include "scheluder.h"

void kernel_main() {
	//add_page();
	init_gdt_kernel();
	pmm_init();
	idt_init();

	int* a = (int*)kmalloc(4);

	*a = 50;

	scheluder_start_first_proc();

	__asm__ volatile("sti");

	while(1);
}

void __stack_chk_fail(void) {
    while(1) {
        __asm__ __volatile__("hlt"); 
    }
}