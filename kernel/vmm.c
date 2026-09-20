#include "types.h"
#include "vmm.h"
#include "pmm.h"

int vmm_map_page(page_table_t* pml4_root, uint64_t virtual_address, uint64_t phys_address) {
	uint16_t pml4_idx = (virtual_address >> 39) & 0x1FF;
	uint16_t pdpt_idx = (virtual_address >> 30) & 0x1FF;
	uint16_t pd_idx = (virtual_address >> 21) & 0x1FF;
	uint16_t pt_idx = (virtual_address >> 12) & 0x1FF;

	if ((pml4_root->entries[pml4_idx] & 0x1) == 0) {
		uint64_t new_table_page = (uint64_t)pmm_alloc_page();
		if (new_table_page == 0) return 0;

		pml4_root->entries[pml4_idx] = new_table_page | 0x3;
	}

	page_table_t* pdpt = phys_to_virt_address(pml4_root->entries[pml4_idx] & ~0xFFF);

	if ((pdpt->entries[pdpt_idx] & 0x1) == 0) {
		uint64_t new_table_page = (uint64_t)pmm_alloc_page();
		if (new_table_page == 0) return 0;

		pdpt->entries[pdpt_idx] = new_table_page | 0x3;
	}

	page_table_t* pd = phys_to_virt_address(pdpt->entries[pdpt_idx] & ~0xFFF);

	if ((pd->entries[pd_idx] & 0x1) == 0) {
		uint64_t new_table_page = (uint64_t)pmm_alloc_page();
		if (new_table_page == 0) return 0;

		pd->entries[pd_idx] = new_table_page | 0x3;
	}

	page_table_t* pt = phys_to_virt_address(pd->entries[pd_idx] & ~0xFFF);

	pt->entries[pd_idx] = phys_address | 0x3;

	__asm__ volatile("invlpg (%0)" :: "r"(virtual_address) : "memory");
	return 1;
}

int vmm_unmap_page(page_table_t* pml4_root, uint64_t virtual_address) {
	uint16_t pml4_idx = (virtual_address >> 39) & 0x1FF;
	uint16_t pdpt_idx = (virtual_address >> 30) & 0x1FF;
	uint16_t pd_idx = (virtual_address >> 21) & 0x1FF;
	uint16_t pt_idx = (virtual_address >> 12) & 0x1FF;

	if ((pml4_root->entries[pml4_idx] & 0x1) == 0) return 0;
	page_table_t* pdpt = phys_to_virt_address(pml4_root->entries[pml4_idx] & ~0xFFF);

	if ((pdpt->entries[pdpt_idx] & 0x1) == 0) return 0;
	page_table_t* pd = phys_to_virt_address(pdpt->entries[pdpt_idx] & ~0xFFF);

	if ((pd->entries[pd_idx] & 0x1) == 0) return 0;
	page_table_t* pt = phys_to_virt_address(pd->entries[pd_idx] & ~0xFFF);

	if ((pd->entries[pd_idx] & 0x1) == 0) return 0;

	uint64_t page_phys = pt->entries[pt_idx] & ~0xFFF;

	pmm_free_page((void*)page_phys);

	pt->entries[pt_idx] = 0;

	__asm__ volatile("invlpg (%0)" :: "r"(virtual_address) : "memory");
	return 1;
}

uint64_t vmm_create_pml4_root(void) {
	uint64_t pml4_root_phys = (uint64_t)pmm_alloc_page();
	if (pml4_root_phys == 0) return 0;

	page_table_t* pml4_root = phys_to_virt_address(pml4_root_phys);

	for (int i = 0; i < ENTRIES_COUNT; i++) {
		pml4_root->entries[i] = 0;
	}

	page_table_t* pml4_current = phys_to_virt_address(vmm_get_current_pml4_address());

	pml4_root->entries[KERNEL_PML4_INDEX] = pml4_current->entries[KERNEL_PML4_INDEX];

	return pml4_root_phys;
}

uint64_t vmm_get_current_pml4_address(void) {
	uint64_t pml4_address;

	 __asm__ volatile("movq %%cr3, %0" : "=r" (pml4_address));

	return pml4_address;
}