#include <k/idt.h>
#include <k/irq.h>
#include <k/kstd.h>
#include <k/types.h>
#include <stdio.h>

#include "../io.h"

#define EXTERN(x) extern void _irq##x();

#define IRQ_NB_TO_BYTE(x) (x + 32)

EXTERN(0)
EXTERN(1)
EXTERN(2)
EXTERN(3)
EXTERN(4)
EXTERN(5)
EXTERN(6)
EXTERN(7)
EXTERN(8)
EXTERN(9)
EXTERN(10)
EXTERN(11)
EXTERN(12)
EXTERN(13)
EXTERN(14)
EXTERN(15)

#define HANDLER(x) (unsigned)_irq##x

// NULL terminated array
static irq_handler_t irq_handlers[IRQ_COUNT] = { NULL };

// *heavily* inspired from osdev implementations
void pic_remap(int offset1, int offset2)
{
    unsigned char a1 = inb(PIC1_DATA);
    PIC_WAIT()
    unsigned char a2 = inb(PIC2_DATA);
    PIC_WAIT()

    // starting initializing sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    PIC_WAIT()
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    PIC_WAIT()

    // ICW2 master and slave offset
    outb(PIC1_DATA, offset1);
    PIC_WAIT()
    outb(PIC2_DATA, offset2);
    PIC_WAIT()

    // ICW3 tell master that there is a slave, tell slave it's a cascade entity
    outb(PIC1_DATA, 4);
    PIC_WAIT()
    outb(PIC2_DATA, 2);
    PIC_WAIT()

    outb(PIC1_DATA, ICW4_8086);
    PIC_WAIT()
    outb(PIC2_DATA, ICW4_8086);
    PIC_WAIT()

    outb(PIC1_DATA, a1);
    PIC_WAIT()
    outb(PIC2_DATA, a2);
    PIC_WAIT()
}

void irq_set_mask(unsigned char irqline)
{
    u16 port;
    u8 value;

    if (irqline < 8)
        port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        value -= 8;
    }
    value = inb(port) | (1 << irqline);
    outb(port, value);
}

void irq_clear_mask(unsigned char irqline)
{
    u16 port;
    u8 value;

    if (irqline < 8)
        port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        value -= 8;
    }
    value = inb(port) & ~(1 << irqline);
    outb(port, value);
}

void irq_ack(u8 irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void irq_init(void)
{
    pic_remap(PIC1_OFFSET, PIC2_OFFSET);
    printf("PIC REMAPPED SUCCESSFULLY\n");

    set_idt_entry(IRQ_NB_TO_BYTE(0), HANDLER(0), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(1), HANDLER(1), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(2), HANDLER(2), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(3), HANDLER(3), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(4), HANDLER(4), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(5), HANDLER(5), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(6), HANDLER(6), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(7), HANDLER(7), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(8), HANDLER(8), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(9), HANDLER(9), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(10), HANDLER(10), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(11), HANDLER(11), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(12), HANDLER(12), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(13), HANDLER(13), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(14), HANDLER(14), 0x08, 0x8E);
    set_idt_entry(IRQ_NB_TO_BYTE(15), HANDLER(15), 0x08, 0x8E);

    irq_set_mask(0);
    irq_set_mask(1);
    irq_set_mask(2);
    irq_set_mask(3);
    irq_set_mask(4);
    irq_set_mask(5);
    irq_set_mask(6);
    irq_set_mask(7);
    irq_set_mask(8);
    irq_set_mask(9);
    irq_set_mask(10);
    irq_set_mask(11);
    irq_set_mask(12);
    irq_set_mask(13);
    irq_set_mask(14);
    irq_set_mask(15);

    RESUME_INT
}

void irq_install_handler(u8 irq, irq_handler_t handler)
{
    STOP_INT
    irq_handlers[irq] = handler;
    irq_clear_mask(irq);
    RESUME_INT
}

void irq_delete_handler(u8 irq)
{
    STOP_INT
    irq_set_mask(irq);
    irq_handlers[irq] = NULL;
    RESUME_INT
}

void irq_handler(regs_t *regs)
{
    STOP_INT
    // substracting offset from the struct
    irq_handler_t handler = irq_handlers[regs->int_no - 32];
    if (handler)
        handler(regs);
    irq_ack(regs->int_no - 32);
    RESUME_INT
}
