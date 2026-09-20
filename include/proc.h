#ifndef PROC_H
#define PROC_H

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
	void* program_counter;
	void* stack_pointer;
};

#endif