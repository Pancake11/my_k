#include <k/gdt.h>
#include <stdio.h>

static struct gdt_entry gdt[6];

void print_gdt_entry(size_t index)
{
    printf("GDT ENTRY %u\r\n", index);
    printf("LIMIT: %x\r\n", gdt[index].limit);
    printf("BASE_1: %x\r\n", gdt[index].base_1);
    printf("BASE_2: %x\r\n", gdt[index].base_2);
    printf("ACCESS: %x\r\n", gdt[index].access);
    printf("LIMIT_FLAGS: %x\r\n", gdt[index].limit_flags);
    printf("BASE_3: %x\r\n\r\n", gdt[index].base_3);
}

void create_gdt_entry(size_t i, u32 base, u32 limit, u8 access, u8 flags)
{
    gdt[i].base_1 = (u16)base;
    gdt[i].base_2 = (u8)(base >> 16);
    gdt[i].base_3 = (u8)(base >> 24);
    gdt[i].limit = (u16)limit;
    gdt[i].access = access;
    gdt[i].limit_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

// setting up segments as specified in the osDev wiki
// and going to protected mode
void gdt_init(void)
{
    // NULL entry
    create_gdt_entry(0, 0, 0, 0, 0);
    // kernel mode code segment
    create_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // kernel mode data segement
    create_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // user mode code segment
    create_gdt_entry(3, 0, 0, 0, 0);
    // user mode data segement
    create_gdt_entry(4, 0, 0, 0, 0);
    // Task state segment (TSS)
    create_gdt_entry(5, 0, 0, 0, 0);

    // printing all the gdt entries
    size_t i = 0;
    while (i < 6)
        print_gdt_entry(i++);

    // setting up the gdtr
    struct gdtr gdtr;
    gdtr.base = (u32)gdt;
    gdtr.limit = sizeof(gdt) - 1;

    // stopping all interrupts
    __asm__ __volatile__("cli");
    // LGDT
    __asm__ __volatile__("lgdt %0\n" : : "m"(gdtr) : "memory");

    // Enable PE
    __asm__ __volatile__("movl %cr0, %eax");
    __asm__ __volatile__("or $1, %al");
    __asm__ __volatile__("movl %eax, %cr0");

    // Segments
    __asm__ __volatile__("movw $0x10, %ax");
    __asm__ __volatile__("movw %ax, %ds");
    __asm__ __volatile__("movw %ax, %es");
    __asm__ __volatile__("movw %ax, %fs");
    __asm__ __volatile__("movw %ax, %gs");
    __asm__ __volatile__("movw %ax, %ss");

    // Switch
    __asm__ __volatile__("ljmp $0x08, $1f");
    __asm__ __volatile__("1:");
}
