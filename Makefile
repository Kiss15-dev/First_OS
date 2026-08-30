ASM = nasm
CC = gcc
LD = ld

CC_FLAGS = -m32 -c -ffreestanding -fno-pie -nostdlib
LD_FLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

IMAGE = os-image.bin

all: $(IMAGE)

$(IMAGE): boot.bin kernel.bin
	cat boot.bin kernel.bin > $(IMAGE)

boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

kernel.bin: kernel_entry.o kernel.o
	$(LD) $(LD_FLAGS) kernel_entry.o kernel.o -o kernel.bin

kernel_entry.o: kernel_entry.asm
	$(ASM) -f elf32 kernel_entry.asm -o kernel_entry.o

kernel.o: kernel.c
	$(CC) $(CC_FLAGS) kernel.c -o kernel.o

run: $(IMAGE)
	qemu-system-i386 -drive format=raw,file=$(IMAGE)

clean:
	rm -f *.o *.bin $(IMAGE)