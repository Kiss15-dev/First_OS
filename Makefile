ASM = nasm
CC = gcc
LD = ld

CC_FLAGS = -c -ffreestanding -fno-pie -nostdlib -m64
LD_FLAGS = -m elf_x86_64 -Ttext 0x9000 -e _start --oformat binary

IMAGE = os-image.bin

all: $(IMAGE)

$(IMAGE): boot.bin kernel.bin
	cat boot.bin kernel.bin > $(IMAGE)

boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

kernel.bin: kernel_entry.o kernel.o idt.o pic.o keyboard.o
	$(LD) $(LD_FLAGS) kernel_entry.o kernel.o idt.o pic.o keyboard.o -o kernel.bin

kernel_entry.o: kernel_entry.asm
	$(ASM) -f elf64 kernel_entry.asm -o kernel_entry.o

kernel.o: kernel.c
	$(CC) $(CC_FLAGS) kernel.c -o kernel.o

pic.o: pic.c
	$(CC) $(CC_FLAGS) pic.c -o pic.o

idt.o: idt.c
	$(CC) $(CC_FLAGS) idt.c -o idt.o

keyboard.o: keyboard.c
	$(CC) $(CC_FLAGS) keyboard.c -o keyboard.o

run: $(IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(IMAGE)

clean:
	rm -f *.o *.bin $(IMAGE)