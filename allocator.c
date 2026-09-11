#include "types.h"
#include "allocator.h"
#include "pmm.h"

struct BlockHeader
{
	uint64_t size_and_flag;
	struct BlockHeader* next;
};

static uintptr_t bump_ptr = 0;
static uintptr_t page_end = 0;
static struct BlockHeader* first_block = NULL;
static struct BlockHeader* current_blockheader = NULL;
static struct BlockHeader* find_block = NULL;

void* add_page() {
	void* page = pmm_alloc_page();

	if (page == NULL) return NULL;

	first_block = (struct BlockHeader*)page;
	first_block->size_and_flag = 0;
	first_block->next = NULL;

	if (current_blockheader != NULL) {
		current_blockheader->next = first_block;
	}

	bump_ptr = (uintptr_t)page + sizeof(struct BlockHeader);
	page_end = (uintptr_t)page + PAGE_SIZE;
	current_blockheader = first_block;
	find_block = first_block;
}	

void* kmalloc(size_t size) {
	size = (size + 15) & ~15;
	int total_size = sizeof(struct BlockHeader) + size;
	void* allocated_memory;
	find_block = first_block;

	if (bump_ptr + total_size > page_end) {
		current_blockheader->size_and_flag = current_blockheader->size_and_flag | 0x1;
		add_page();
	}

	while (find_block != NULL) {
		if (find_block->size_and_flag & 0x1) {
			find_block = find_block->next;
			continue;
		}

		if (find_block->size_and_flag >= size) {
			allocated_memory = (void*)(find_block + 1);
			find_block->size_and_flag = find_block->size_and_flag | 0x1;

			return allocated_memory;
		} else find_block = find_block->next;
	}

	allocated_memory = (void*)bump_ptr;
	current_blockheader->size_and_flag = size;
	current_blockheader->size_and_flag = current_blockheader->size_and_flag | 0x1;

	struct BlockHeader* new_header = (struct BlockHeader*)(bump_ptr + size);
	new_header->size_and_flag = 0;
	new_header->next = NULL;

	current_blockheader->next = new_header;
	current_blockheader = new_header;
	bump_ptr += total_size;

	return allocated_memory;
}

void kfree(void* allocated_ptr) {
	if (allocated_ptr == NULL) return;

	struct BlockHeader* ptr = (struct BlockHeader*)allocated_ptr;

	ptr -= 1;

	ptr->size_and_flag = ptr->size_and_flag & ~0x1;
}