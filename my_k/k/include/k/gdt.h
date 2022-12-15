#pragma once

#include <k/kfs.h>
#include <k/types.h>

struct gdt_entry
{
    u16 limit;
    u16 base_1;
    u8 base_2;
    u8 access;
    u8 limit_flags;
    u8 base_3;
} __packed;

struct gdtr
{
    // size of the gdtr
    u16 limit;
    // linear address of the GDT (with paging (when it's done))
    u32 base;
} __packed;

void gdt_init(void);
