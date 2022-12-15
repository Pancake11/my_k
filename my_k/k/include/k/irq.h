#pragma once

#include <k/sys.h>
#include <k/idt.h>
#include <k/kstd.h>
#include <k/types.h>

#include "../io.h"

#define IRQ_COUNT 16

#define RESUME_INT asm volatile("sti");
#define STOP_INT asm volatile("cli");

// PIC

#define PIC1 0x20
#define PIC1_COMMAND PIC1
#define PIC1_OFFSET 0x20
#define PIC1_DATA (PIC1 + 1)

#define PIC2 0xA0
#define PIC2_COMMAND PIC2
#define PIC2_OFFSET 0x28
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

#define PIC_WAIT()                                                             \
    asm volatile("jmp 1f\n\t"                                                  \
                 "1:\n\t"                                                      \
                 "    jmp 2f\n\t"                                              \
                 "2:");

// OFFSETS

#define ICW1_ICW4 0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08 /* Level triggered (edge) mode */
#define ICW1_INIT 0x10 /* Initialization - required! */

#define ICW4_8086 0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10 /* Special fully nested (not) */

// Typedef

typedef void (*irq_handler_t)(regs_t *regs);

// Functions

void irq_init();
void irq_install_handler(u8 irq, irq_handler_t handler);
void irq_uninstall_handler(u8 irq);
void irq_ack(u8 irq);
