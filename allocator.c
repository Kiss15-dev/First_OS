#include "types.h"
#include "allocator.h"
#include "pmm.h"

static uintptr_t bump_ptr = 0;
static uintptr_t page_end = 0;

void* kmalloc(size_t size) {
	size = (size + 15) & ~15;

	if (bump_ptr + size > page_end) {
		void* page = pmm_alloc_page();

		if (page == NULL) return NULL;

		bump_ptr = (uintptr_t)page;
		page_end = (uintptr_t)page + PAGE_SIZE;
	}

	void* allocated_memory = (void*)bump_ptr;
	bump_ptr = bump_ptr + size;

	return allocated_memory;
}