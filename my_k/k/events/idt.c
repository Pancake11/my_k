#include <k/idt.h>
#include <k/kstd.h>
#include <k/types.h>
#include <stdio.h>
#include <string.h>

struct idt_entry idt[IDT_ENTRY_COUNT];

void print_idt(int n)
{
    printf("IDT ENTRY : %u\r\n", n);
}

void set_idt_entry(int n, u32 offset, u16 selector, u8 flags)
{
    idt[n].offset_1 = (u16)offset;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].type_attributes = flags;
    idt[n].offset_2 = (u16)(offset >> 16);
    print_idt(n);
}

void idt_init(void)
{
    struct idtr idtr;
    idtr.base = (u32)idt;
    idtr.limit = sizeof(idt) - 1;
    memset(idt, 0, sizeof(idt));

    __asm__ __volatile__("lidt %0\n" ::"m"(idtr) : "memory");
}
