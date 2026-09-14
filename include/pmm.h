#ifndef PMM_H
#define PMM_H

#define HEAP_START 0x100000
#define PAGE_SIZE 4096
#define HEAP_SIZE (4096 * 200)
#define HEAP_END (HEAP_START + HEAP_SIZE)

void pmm_init(void);
void* pmm_alloc_page(void);
void pmm_free_page(void* page_address);

#endif