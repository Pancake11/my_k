;dont touch to the labels they are needed to do some jumps
gdt_start:
    ;null sector
    dd 0x0
    dd 0x0

;GDT for code and data segment, base = 0x0000000, length = 0xffff
;for flags refer to os-dev pdf document page 36
gdt_code:
    dw 0xffff   ;segement length, bits 0-15
    dw 0x0      ;segment base
    db 0x0      ;segment base
    db 10011010b; flags
    db 11001111b; flags + segment length
    db 0x0      ;segment base

gdt_data:
    dw 0xffff   ;segement length, bits 0-15
    dw 0x0      ;segment base
    db 0x0      ;segment base
    db 10010010b; flags
    db 11001111b; flags + segment length
    db 0x0      ;segment base

gdt_end:

;GDT description
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

;define some cst
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
