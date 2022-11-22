/*
 * =====================================================================================
 *
 *       Filename:  serial.c
 *
 *    Description: implementation of the serial syscall write that you can find
 * in basic 8086 cpu architectures is is strictly following the manpage
 * specifications
 *
 *        Version:  1.0
 *        Created:  20.11.2022 16:17:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maxim payeur
 *
 * =====================================================================================
 */
#include <k/serial.h>
#include <k/kstd.h>

#include "../io.h"

#define COMM1 0x3F8

static int is_transmit_empty(void)
{
    return inb(COMM1 + 5) & 0x20;
}

static void write_char(char a)
{
    while (is_transmit_empty() == 0)
        ;

    outb(COMM1, a);
}

int write(char *buf, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        write_char(buf[i]);
    }

    // EOL
        write_char('\r');
        write_char('\n');
    return 0;
}

// setting UART 16550 to 38400 bps
// and initializing the COMM1
int init_comm1(void)
{
    // setting up the bps, fifo and other misc things
    outb(COMM1 + 1, 0x00);
    outb(COMM1 + 3, 0x80);
    outb(COMM1 + 0, 0x03);
    outb(COMM1 + 1, 0x00);
    outb(COMM1 + 3, 0x03);
    outb(COMM1 + 2, 0xC7);
    outb(COMM1 + 4, 0x0B);
    outb(COMM1 + 4, 0x1E);
    outb(COMM1 + 0, 0xAE);

    // check if serial is faulty
    if (inb(COMM1 + 0) != 0xAE)
        return 1;

    // if serial is not faulty set evrything back
    outb(COMM1 + 4, 0x0F);
    return 0;
}
