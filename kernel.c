void kernel_main() {
	char* video_memory = (char*)0xB8000;	

	for (int i = 0; i < 80 * 25 * 2; ) {
		video_memory[i] = ' ';
		i += 2;
	}

	while(1);
}