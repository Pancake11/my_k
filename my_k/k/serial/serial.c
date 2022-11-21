/*
 * =====================================================================================
 *
 *       Filename:  serial.c
 *
 *    Description: implementation of the serial syscall write that you can find in basic
 *                  8086 cpu architectures is is strictly following the manpage specifications
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
#include "../io.h"

#include <k/serial.h>

#define COMM1 0x3F8

int write(char *buf, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        outb(COMM1, buf[i]);
    }

    //EOL
    outb(COMM1, '\r');
    outb(COMM1, '\n');
    return 0;
}

//setting UART 16550 to 38400 bps
//and initializing the COMM1
void init_comm1(void)
{
    //setting up the bps
    outb(COMM1, 0x03);
    outb(COMM1 + 1, 0x00);
}
