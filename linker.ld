ENTRY(_start)

SECTIONS
{
    . = 0xFFFFFFFF80009000;

    .text : AT(0x9000)
    {
        *(.text)                  /* Сюда собирается весь код */
    }

    .data : AT(ADDR(.data) - 0xFFFFFFFF80000000)
    {
        *(.data)
        *(.rodata)                /* Строковые константы */
    }

    .bss : AT(ADDR(.bss) - 0xFFFFFFFF80000000)
    {
        *(.bss)
    }

    _kernel_end = .;
}
