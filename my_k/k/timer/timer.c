#include <k/idt.h>
#include <k/irq.h>
#include <k/kstd.h>
#include <k/sys.h>
#include <k/timer.h>
#include <k/types.h>
#include <stdio.h>

static unsigned long tick = 0;

unsigned long gettick(void)
{
    return tick;
}

void timer_handler(regs_t *regs)
{
    (void)regs;
    tick++;
}

void timer_init(void)
{
    // 0x34 = 0011 0100
    // that's channel 0 access mode low/high
    // with mode 2

    outb(CONTROL, 0x34);
    u16 divisor = BASE_FREQ / FREQ;

    outb(CHANNEL0, (u8)divisor);
    outb(CHANNEL0, (u8)(divisor >> 8));

    irq_install_handler(TIMER_IRQ, timer_handler);
}
