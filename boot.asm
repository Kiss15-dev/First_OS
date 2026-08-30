[org 0x7c00]
[bits16]

start:
	cli 
	lgdt [gdt_descriptor]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:init_pm

[bits 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	jmp $

gdt_start:
    ; Каждая запись (дескриптор) занимает 8 байт
    dd 0x0, 0x0         ; Обязательный пустой дескриптор (Null Descriptor)

gdt_code:               ; Дескриптор сегмента кода
    ; База = 0x0, Лимит = 0xfffff (при гранулярности 4КБ это дает все 4 ГБ)
    dw 0xffff           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10011010b        ; Flags: Present, Ring 0, Code, Executable, Readable
    db 11001111b        ; Granularity (4KB), 32-bit mode, Limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)

gdt_data:               ; Дескриптор сегмента данных
    dw 0xffff           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10010010b        ; Flags: Present, Ring 0, Data, Writable
    db 11001111b        ; Granularity (4KB), 32-bit mode, Limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)
gdt_end:

; Описатель GDT для инструкции lgdt
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Размер GDT минус 1 байт
    dd gdt_start               ; Физический адрес начала таблицы

; Константы для селекторов (смещения внутри GDT)
CODE_SEG equ gdt_code - gdt_start ; Вычисляется как 0x08
DATA_SEG equ gdt_data - gdt_start ; Вычисляется как 0x10


times 510 - ($ - $$) db 0
dw 0xaa55