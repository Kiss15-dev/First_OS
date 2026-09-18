[org 0x7c00]
KERNEL_OFFSET equ 0x9000
KERNEL_SECTORS equ 100

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

    jmp PM_CODE_SEG:init_pm

disk_error:
    jmp $

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x500000
    mov ebp, esp

    mov edi, 0x20000      
    mov cr3, edi          
    xor eax, eax          
    mov ecx, 527360
    rep stosd             

    mov dword [0x20000], 0x21000 | 0x03
    mov dword [0x20000 + 4], 0
    mov dword [0x20000 + 511 * 8], 0x21000 | 0x03
    mov dword [0x20000 + 511 * 8 + 4], 0

    mov dword [0x21000], 0x22000 | 0x03 
    mov dword [0x21000 + 4], 0

    mov edi, 0x22000
    mov ebx, 0x23000 | 0x03
    mov ecx, 512
.loop_fill_pd:
    mov dword [edi], ebx
    mov dword [edi+4], 0  
    add ebx, 4096         
    add edi, 8            
    loop .loop_fill_pd

    mov edi, 0x23000       
    mov ebx, 0x00000003   
    mov ecx, 262144    
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

    jmp LM_CODE_SEG:init_lm

[bits 64]
init_lm:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0xFFFFFFFF80500000
    mov rbp, rsp

    mov rbx, 0xFFFFFFFF80000000 + KERNEL_OFFSET
    jmp rbx

align 8
gdt_start:
    dq 0x0                  

gdt_pm_code:               
    dw 0xFFFF               
    dw 0x0000               
    db 0x00                 
    db 10011010b            ; Present, Ring 0, Code, Executable, Readable
    db 11001111b            ; G=1 (4KB granularity), D=1 (32-bit mode) <--- ТУТ ИЗМЕНЕНО
    db 0x00                

gdt_data:               
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b            
    db 11001111b            
    db 0x00

gdt_lm_code:               
    dw 0xFFFF               
    dw 0x0000               
    db 0x00                 
    db 10011010b            ; Present, Ring 0, Code, Executable, Readable
    db 00101111b            ; L=1 (64-bit Long Mode code segment), D=0
    db 0x00   
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 
    dd gdt_start            

PM_CODE_SEG equ gdt_pm_code - gdt_start 
DATA_SEG equ gdt_data - gdt_start
LM_CODE_SEG equ gdt_lm_code - gdt_start 

times 510 - ($ - $$) db 0
dw 0xaa55