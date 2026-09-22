#include "gdt.h"
#include "types.h"
#include "string.h"

struct GDT_Descriptor gdt[GDT_ENTRIES];
struct GDT_Pointer gdt_ptr;
struct TSS tss_entry;

void set_gdt_gate(int num, uint8_t access, uint8_t gran) {
	gdt[num].limit_low = 0;
	gdt[num].base_low = 0;
	gdt[num].base_middle = 0;
	gdt[num].base_high = 0;
	gdt[num],access = access;
	gdt[num].granularity = gran;
}

void set_gdt_tss(int num, uint64_t tss_address, uint32_t tss_size) {
	struct GDT_TSS_Descriptor* tss_desc = (struct GDT_TSS_Descriptor*)&gdt[num];

	tss_desc->common.limit_low = (tss_size - 1) & 0xFFFF;
	tss_desc->common.base_low = tss_address & 0xFFFF;
	tss_desc->common.base_middle = (tss_address >> 16) & 0xFF;
	tss_desc->common.access = 0x89;
	tss_desc->common.granularity = ((tss_size - 1) >> 16) & 0x0F;
	tss_desc->common.base_high = (tss_address >> 24) & 0xFF;

	tss_desc->base_highest = (tss_address >> 32) & 0xFFFFFFFF;
	tss_desc->reserved = 0;
}

void init_gdt_kernel(void) {
	gdt_ptr.limit = (sizeof(struct GDT_Descriptor) * GDT_ENTRIES) - 1;
	gdt_ptr.base = (uint64_t)&gdt;

	set_gdt_gate(0, 0, 0);
	set_gdt_gate(1, 0x9A, 0x20);
	set_gdt_gate(2, 0x92, 0x00);
	set_gdt_gate(3, 0xFA, 0x20);
	set_gdt_gate(4, 0xF2, 0x00);

	my_memset(&tss_entry, 0, sizeof(struct TSS));

	uint64_t tss_address = (uint64_t)&tss_entry;
	set_gdt_tss(5, tss_address, sizeof(struct TSS));

	__asm__ volatile("lgdt %0" :: "m"(gdt_ptr));
	__asm__ volatile("ltr %%ax" :: "a"(0x28));
}