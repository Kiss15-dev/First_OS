#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "types.h"

void* add_page();
void kallocator_init(void);
void* kmalloc(size_t size);
void* krealloc(void* allocated_ptr, size_t new_size);
void kfree(void* allocated_ptr);

#endif