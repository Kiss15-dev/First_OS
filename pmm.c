#include "types.h"
#include "pmm.h"

static void* next_page = (void*)HEAP_START;

void* pmm_alloc_page() {
	uint8_t* page = (uint8_t*)next_page;
	
	if ((uintptr_t)page + PAGE_SIZE <= (uintptr_t)HEAP_END) {
		next_page = (uint8_t*)next_page + PAGE_SIZE;

		return (void*)page;
	}

	return NULL;
}