#include "allocator.h"
#include "string.h"
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
	if (size <= 0) return NULL;
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

void* krealloc(void* allocated_ptr, size_t new_size) {
	if (new_size <= 0) return allocated_ptr;

	new_size = (new_size + 15) & ~15;

	void* ptr = allocated_ptr;
	struct BlockHeader* allocated_block = (struct BlockHeader*)((uintptr_t)allocated_ptr - sizeof(struct BlockHeader));

	if ((allocated_block->size_and_flag & ~0x1) == new_size) return allocated_ptr;

	if (new_size < allocated_block->size_and_flag & ~0x1) {
		allocated_block->size_and_flag = allocated_block->size_and_flag & ~0x1;
		allocated_block->size_and_flag = new_size | 0x1;

		return allocated_ptr;
	}

	if (new_size > allocated_block->size_and_flag & ~0x1) {
		uint64_t sum_size;
		struct BlockHeader* next_block = allocated_block;

		while (next_block->next != NULL && next_block->size_and_flag & 0x1 != 1) {
			next_block = next_block->next;
			sum_size += next_block->size_and_flag + sizeof(struct BlockHeader);	

			if (sum_size >= new_size) {
				allocated_block->size_and_flag = allocated_block->size_and_flag & ~0x1;
				allocated_block->size_and_flag = sum_size | 0x1;

				if (next_block->next == NULL) {
					current_blockheader = (struct BlockHeader*)((uintptr_t)next_block + sizeof(struct BlockHeader));
					bump_ptr = (uintptr_t)current_blockheader + sizeof(struct BlockHeader);
					allocated_block->next = current_blockheader;

					return allocated_ptr;
				} else {
					allocated_block->next = next_block->next;
				}

				return allocated_ptr;
			}
		}

		void* new_allocated_ptr = kmalloc(new_size);

		my_memcpy(new_allocated_ptr, ptr, (allocated_block->size_and_flag & ~0x1));
		allocated_block->size_and_flag = allocated_block->size_and_flag & ~0x1;

		return new_allocated_ptr;
	}
}

void kfree(void* allocated_ptr) {
	if (allocated_ptr == NULL) return;

	struct BlockHeader* ptr = (struct BlockHeader*)allocated_ptr;

	ptr -= 1;

	ptr->size_and_flag = ptr->size_and_flag & ~0x1;
}