#ifndef GDT_H
#define GDT_H

#include "types.h"

#define GDT_ENTRIES 7

struct GDT_Descriptor
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct GDT_TSS_Descriptor {
	struct GDT_Descriptor common;
	uint32_t base_highest;
	uint32_t reserved;
} __attribute__((packed));

struct GDT_Pointer {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

struct TSS {
	uint32_t reserved0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved1;
	uint64_t ist[7];
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t iomap_base;
} __attribute__((packed));

void init_gdt_kernel(void);

#endif