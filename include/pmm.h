#ifndef PMM_H
#define PMM_H

#define HEAP_START (0xFFFFFFFF80000000ULL + 0x700000ULL)
#define PAGE_SIZE 4096
#define HEAP_SIZE (PAGE_SIZE * 51200)
#define HEAP_END (HEAP_START + HEAP_SIZE)
#define USER_POOL_START (HEAP_END)
#define USER_POOL_SIZE (PAGE_SIZE * 51200)
#define USER_POOL_END (USER_POOL_START + USER_POOL_SIZE)

void pmm_init(void);
void* pmm_alloc_page(void);
void pmm_free_page(void* page_address);

#endif