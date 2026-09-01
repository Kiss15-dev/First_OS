#ifndef IDT_H
#define IDT_H

#include "types.h"

typedef struct 
{
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t ist;
	uint8_t attributes;
	uint16_t isr_mid;
	uint32_t isr_high;
	uint32_t reserved;
} __attribute__((packed)) idt_entity_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idtr_t;

void idt_init(void);

#endif