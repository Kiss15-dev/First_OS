#include "scheluder.h"
#include "io.h"
#include "types.h"
#include "allocator.h"
#include "gdt.h"
#include "vmm.h"

_Noreturn void start_first_process(uint64_t stack_pointer, uint64_t pml4_root);

struct proc* current_proc = NULL;

void scheluder_start_first_proc() {
	uint64_t pml4_phys = vmm_create_pml4_root();
	current_proc = proc_create(pml4_phys, 0);
	if (current_proc == NULL) return;

	update_tss_rsp0((uint64_t)current_proc->kstack_top);

	start_first_process(current_proc->rsp, current_proc->pml4_root);
}

void scheluder() {
	

	outb(0x20, 0x20);
}