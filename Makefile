ASM = nasm
CC = gcc
LD = ld

CC_FLAGS = -c -ffreestanding -fno-pic -fno-pie -mcmodel=kernel -mno-red-zone -nostdlib -m64 -Iinclude
LD_FLAGS = -m elf_x86_64 -T linker.ld --oformat binary

B = boot
K = kernel
D = drivers
L = lib

IMAGE = os-image.bin

all: $(IMAGE)

$(IMAGE): boot.bin kernel.bin
	cat boot.bin kernel.bin > $(IMAGE)

boot.bin: $B/boot.asm
	$(ASM) -f bin $B/boot.asm -o boot.bin

kernel.bin: $B/kernel_entry.o $K/kernel.o $K/idt.o $K/pic.o $D/keyboard.o $K/allocator.o $K/pmm.o $K/vmm.o $K/proc.c $L/string.o $L/vector.o 
	$(LD) $(LD_FLAGS) $B/kernel_entry.o $K/kernel.o $K/idt.o $K/pic.o $D/keyboard.o $K/allocator.o $K/pmm.o $K/vmm.o $K/proc.c $L/string.o $L/vector.o -o kernel.bin

$B/kernel_entry.o: $B/kernel_entry.asm
	$(ASM) -f elf64 $B/kernel_entry.asm -o $B/kernel_entry.o

$K/kernel.o: $K/kernel.c
	$(CC) $(CC_FLAGS) $K/kernel.c -o $K/kernel.o

$K/pic.o: $K/pic.c
	$(CC) $(CC_FLAGS) $K/pic.c -o $K/pic.o

$K/idt.o: $K/idt.c
	$(CC) $(CC_FLAGS) $K/idt.c -o $K/idt.o

$D/keyboard.o: $D/keyboard.c
	$(CC) $(CC_FLAGS) $D/keyboard.c -o $D/keyboard.o

$K/allocator.o: $K/allocator.c
	$(CC) $(CC_FLAGS) $K/allocator.c -o $K/allocator.o

$K/vmm.o: $K/vmm.c
	$(CC) $(CC_FLAGS) $K/vmm.c -o $K/vmm.o

$K/proc.o: $K/proc.c
	$(CC) $(CC_FLAGS) $K/proc.c -o $K/proc.o

$K/pmm.o: $K/pmm.c
	$(CC) $(CC_FLAGS) $K/pmm.c -o $K/pmm.o

$L/string.o: $L/string.c
	$(CC) $(CC_FLAGS) $L/string.c -o $L/string.o

$L/vector.o: $L/vector.c
	$(CC) $(CC_FLAGS) $L/vector.c -o $L/vector.o

run: $(IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(IMAGE)

clean:
	rm -f *.o *.bin $(IMAGE)