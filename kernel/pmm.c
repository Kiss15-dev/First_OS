#include "types.h"
#include "pmm.h"
#include "allocator.h"

struct Page
{
	struct Page* next;
};

//static void* next_page = (void*)HEAP_START;

static struct Page* free_pages_head = NULL;

void pmm_init() {
	for (uintptr_t page_address = USER_POOL_START; page_address + PAGE_SIZE <= USER_POOL_END; page_address += PAGE_SIZE) {
		pmm_free_page((void*)page_address);
	}

	kallocator_init();
}

void* pmm_alloc_page() {
	/*if (next_page == NULL) return NULL;
	void* page = next_page;
	
	if ((uintptr_t)page + PAGE_SIZE <= (uintptr_t)HEAP_END) {
		next_page = (void*)((uintptr_t)next_page + PAGE_SIZE);
	} else {
		next_page = NULL;
	}

	return page;*/
	if (free_pages_head == NULL) return NULL;

	struct Page* allocated_memory = free_pages_head;

	free_pages_head = free_pages_head->next;

	return (void*)allocated_memory;
}

void pmm_free_page(void* page_address) {
	if (page_address == NULL) return;

	struct Page* released_page = (struct Page*)page_address;

	released_page->next = free_pages_head;

	free_pages_head = released_page;
}