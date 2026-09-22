#ifndef PROC_H
#define PROC_H

#include "types.h"

#define CODE_SIZE_DIRECTORY 1024 * 64
#define CODE_START 0x2000
#define CODE_END (0x2000 + CODE_SIZE_DIRECTORY)
#define STACK_POINTER 0x00007FFFFFFFFFFF

typedef struct {
	uint64_t rax; uint64_t rbx; uint64_t rcx; 
	uint64_t rdx; uint64_t rbp; uint64_t rsi;
	uint64_t rdi; uint64_t r8; uint64_t r9;
	uint64_t r10; uint64_t r11; uint64_t r12;
	uint64_t r13; uint64_t r14; uint64_t r15;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} context_t;

enum proc_state
{
	UNUSED,
	READY,
	RUNNING,
	BLOCKED,
};

struct proc
{
	int pid;
	enum proc_state state;	
	void* kstack;
	void* ustack;
	uint64_t pml4_root;
	context_t context;
};

//extern struct proc proc_list;

void proc_list_init(void);
struct proc* proc_create(uint64_t pml4_root_phys, uint64_t entry_point, uint16_t cs, uint16_t ss);

#endif