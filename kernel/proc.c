#include "vmm.h"
#include "allocator.h"
#include "types.h"
#include "proc.h"

struct proc* proc_list = NULL;

void proc_list_init(void) {
	struct proc* proc_list = (struct proc)kmalloc(sizeof(struct proc) * 64);

	for (int i = 0; i < 64; i++) {
		proc_list[i]->pid = i;
		proc_list[i]->state = UNUSED;
		proc_list[i]->program_counter = NULL;
		proc_list[i]->stack_pointer = NULL;
		proc_list[i]->pml4_root = NULL;
	}
}

struct proc proc_create(uint64_t pml4_root_phys, uint64_t entry_point) {
	struct proc proc = &proc_list;

	
}