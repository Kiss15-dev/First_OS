#include "types.h"
#include "pmm.h"

static void* next_page = (void*)HEAP_START;

void* pmm_alloc_page() {
	void* page = next_page;
	
	if ((uintptr_t)page + PAGE_SIZE <= (uintptr_t)HEAP_END) {
		next_page = (void*)((uintptr_t)next_page + PAGE_SIZE);

		return page;
	}

	return NULL;
}