#pragma once

#include <k/kfs.h>
#include <k/kstd.h>
#include <k/types.h>

#define IDT_ENTRY_COUNT 100

// struct comes from osdev example
struct idt_entry
{
    u16 offset_1;
    u16 selector;
    u8 zero;
    u8 type_attributes;
    u16 offset_2;
} __packed;

struct idtr
{
    u16 limit;
    u32 base;
} __packed;

void set_idt_entry(int n, u32 offset, u16 select, u8 flags);
void idt_init(void);
