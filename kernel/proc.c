#include "vmm.h"
#include "allocator.h"
#include "pmm.h"
#include "types.h"
#include "proc.h"

extern void user_test();

struct proc* proc_list = NULL;

void proc_list_init(void) {
	proc_list = kmalloc(sizeof(struct proc) * 64);
	if (proc_list == NULL) return;

	for (int i = 0; i < 64; i++) {
		proc_list[i].pid = i;
		proc_list[i].state = UNUSED;
		proc_list[i].kstack_top = NULL;
		proc_list[i].rsp = 0;
		proc_list[i].pml4_root = 0;
	}
}

void* search_unused_proc() {
	for (int i = 0; i < 64; i++) {
		if (proc_list[i].state == UNUSED) return &proc_list[i];
	}

	return NULL;
}

struct proc* proc_create(uint64_t pml4_root_phys, uint64_t entry_point) {
	struct proc* new_proc = search_unused_proc();
	uint8_t* kstack = (uint8_t*)pmm_alloc_page();
	if (new_proc == NULL) return NULL;

	new_proc->kstack_top = kstack + PAGE_SIZE;
	new_proc->pml4_root = vmm_create_pml4_root();
	uint64_t* rsp = (uint64_t*)new_proc->kstack_top;

	vmm_map_page((page_table_t*)new_proc->pml4_root, 0x7FFFFFFFE000, (uint64_t)pmm_alloc_page(), PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
	vmm_map_page((page_table_t*)new_proc->pml4_root, 0x1000, (uint64_t)pmm_alloc_page(), PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
	vmm_map_page((page_table_t*)new_proc->pml4_root, (uint64_t)(VIRTUAL_KERNEL_OFFSET + (uintptr_t)kstack), (uint64_t)kstack, PAGE_PRESENT | PAGE_WRITABLE);

	rsp--; *rsp = 0x23;
	rsp--; *rsp = 0x7FFFFFFFF000;
	rsp--; *rsp = 0x202;
	rsp--; *rsp = 0x1B;
	rsp--; *rsp = 0x1000;

	for (int i = 0; i < 15; i++) {
		rsp--;
		*rsp = 0;
	}

	new_proc->rsp = (uint64_t)rsp;
	new_proc->state = READY;

	return new_proc;
}