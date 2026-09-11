#include "types.h"
#include "allocator.h"

#define HEAP_START 0x100000
#define HEAP_SIZE (512*1024)
#define HEAP_END (HEAP_START + HEAP_SIZE)

void* next_address = (void*)HEAP_START;

void* malloc(size_t size) {
	size = (size + 15) & ~15;

	uint8_t* current_ptr = (uint8_t*)next_address;

	if ((void*)(current_ptr + size) <= (void*)HEAP_END) {
		next_address = (uint8_t*)next_address + size;

		return current_ptr;
	}

	return NULL;
}