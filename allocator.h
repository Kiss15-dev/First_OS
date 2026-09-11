#ifndef ALLOCATOR_H
#define ALLOCATOR_H

void add_page();
void* kmalloc(size_t size);
void kfree(void* allocated_ptr);

#endif