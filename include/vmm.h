#ifndef VMM_H
#define VMM_H

#define VIRTUAL_KERNEL_OFFSET 0xFFFFFFFF80000000ULL
#define KERNEL_PML4_INDEX 511
#define ENTRIES_COUNT 512
#define PAGE_PRESENT (1ULL << 0)
#define PAGE_WRITABLE (1ULL << 1)
#define PAGE_USER (1ULL << 2) 

#include "types.h"

typedef struct
{	
	uint64_t entries[512];
} page_table_t;

static inline page_table_t* phys_to_virt_address(uint64_t phys_address) {
	return (page_table_t*)(VIRTUAL_KERNEL_OFFSET + phys_address);
}

int vmm_map_page(page_table_t* pml4_root, uint64_t virtual_address, uint64_t phys_address, uint16_t flags);
int vmm_unmap_page(page_table_t* pml4_root, uint64_t virtual_address);
uint64_t vmm_create_pml4_root(void);
uint64_t vmm_get_current_pml4_address(void);

#endif