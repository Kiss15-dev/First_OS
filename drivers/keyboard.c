#include "types.h"
#include "io.h"

const char ascii_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void keyboard_handler() {
	uint8_t scancode = inb(0x60);

	char* video_memory = (char*)0xB8000;

	if (scancode < 0x80) {
		video_memory[0] = ascii_map[scancode];
	}

	outb(0x20, 0x20);
}