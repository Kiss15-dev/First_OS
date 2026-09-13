#include "types.h"
#include "io.h"
#include "idt.h"
#include "pic.h"

extern void keyboard_isr(void);
extern void timer_isr(void);

static idt_entity_t idt[256];
static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entity_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint64_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x08;
	descriptor->ist = 0;
	descriptor->reserved = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
}

void idt_init(void) {
	idtr.base = (uintptr_t)idt;
	idtr.limit = (uint16_t)sizeof(idt_entity_t) * 256 - 1;

	for (int i = 0; i < 256; i++) {
		idt_set_descriptor(i, 0, 0);
	}

	pic_remap(0x20, 0x28);
	
	idt_set_descriptor(0x20, timer_isr, 0x8E);
	idt_set_descriptor(0x21, keyboard_isr, 0x8E);

	__asm__ volatile("lidt %0" : : "m"(idtr));
}