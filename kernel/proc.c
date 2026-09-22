#include "vmm.h"
#include "allocator.h"
#include "pmm.h"
#include "types.h"
#include "proc.h"

struct proc* proc_list = NULL;

void proc_list_init(void) {
	proc_list = kmalloc(sizeof(struct proc) * 64);
	if (proc_list == NULL) return;

	for (int i = 0; i < 64; i++) {
		proc_list[i].pid = i;
		proc_list[i].state = UNUSED;
		proc_list[i].kstack = NULL;
		proc_list[i].ustack = NULL;
		proc_list[i].pml4_root = 0;
	}
}

void* search_unused_proc() {
	for (int i = 0; i < 64; i++) {
		if (proc_list[i].state == UNUSED) return &proc_list[i];
	}

	return NULL;
}

struct proc* proc_create(uint64_t pml4_root_phys, uint64_t entry_point, uint16_t cs, uint16_t ss) {
	struct proc* new_proc = search_unused_proc();
	if (new_proc == NULL) return NULL;

	new_proc->kstack = (void*)pmm_alloc_page();
	new_proc->pml4_root = vmm_create_pml4_root();

	vmm_map_page((page_table_t*)new_proc->pml4_root, 0x7FFFFFFFE000, (uint64_t)pmm_alloc_page(), PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
	vmm_map_page((page_table_t*)new_proc->pml4_root, VIRTUAL_KERNEL_OFFSET + (uint64_t)new_proc->kstack, (uint64_t)new_proc->kstack, PAGE_PRESENT | PAGE_WRITABLE);
	new_proc->ustack = (void*)0x7FFFFFFFE000;

	new_proc->context.rip = entry_point;
	new_proc->context.rsp = 0x7FFFFFFFF000;
	new_proc->context.cs = cs;
	new_proc->context.ss = ss;
	new_proc->context.rflags = 0x202;

	return new_proc;
}