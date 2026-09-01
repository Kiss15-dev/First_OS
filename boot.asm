[org 0x7c00]
KERNEL_OFFSET equ 0x9000
KERNEL_SECTORS equ 150

[bits 16]
start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov bx, KERNEL_OFFSET
    mov al, KERNEL_SECTORS
    mov ch, 0 
    mov dh, 0 
    mov cl, 2 
    mov ah, 0x02 
    int 0x13
    jc disk_error          

    in al, 0x92
    or al, 2
    out 0x92, al

    cli 
    lgdt [gdt_descriptor]
 
    mov eax, cr0 
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

disk_error:
    jmp $

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x80000
    mov ebp, esp

    mov edi, 0x1000       
    mov cr3, edi          
    xor eax, eax          
    mov ecx, 4096         
    rep stosd             

    mov dword [0x1000], 0x2000 | 0x03 
    mov dword [0x2000], 0x3000 | 0x03 
    mov dword [0x3000], 0x4000 | 0x03 

    mov edi, 0x4000       
    mov ebx, 0x00000003   
    mov ecx, 512          
.loop_fill_pt:
    mov dword [edi], ebx
    mov dword [edi+4], 0  
    add ebx, 4096         
    add edi, 8            
    loop .loop_fill_pt

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8        
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp CODE_SEG:init_lm

[bits 64]
init_lm:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0x80000
    mov rbp, rsp

    mov rbx, KERNEL_OFFSET
    jmp rbx

align 8
gdt_start:
    dq 0x0                  

gdt_code:               
    dw 0xFFFF               
    dw 0x0000               
    db 0x00                 
    db 10011010b            
    db 00101111b            
    db 0x00                 

gdt_data:               
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b            
    db 11001111b            
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 
    dd gdt_start            

CODE_SEG equ gdt_code - gdt_start 
DATA_SEG equ gdt_data - gdt_start 

times 510 - ($ - $$) db 0
dw 0xaa55