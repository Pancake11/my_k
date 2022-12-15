#include <k/idt.h>
#include <k/irq.h>
#include <k/keyboard.h>
#include <k/kstd.h>
#include <k/sys.h>
#include <k/types.h>
#include <stdio.h>

#define BUFFER_SIZE 255
#define SC_MAX 57

int front, rear, size = 0;
u8 buffer[BUFFER_SIZE + 1];

const char sc_ascii[] = { '?',  '?', '1', '2',  '3', '4', '5', '6', '7', '8',
                          '9',  '0', '-', '=',  '?', '?', 'Q', 'W', 'E', 'R',
                          'T',  'Y', 'U', 'I',  'O', 'P', '[', ']', '?', '?',
                          'A',  'S', 'D', 'F',  'G', 'H', 'J', 'K', 'L', ';',
                          '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N',
                          'M',  ',', '.', '/',  '?', '?', '?', ' ' };

int is_buffer_empty(void)
{
    return size == 0;
}

int is_buffer_full(void)
{
    return size == BUFFER_SIZE;
}

void enqueue_key(u8 c)
{
    if (is_buffer_full())
        front = (front + 1) % BUFFER_SIZE;
    else
        size++;

    buffer[rear] = sc_ascii[c];
    rear = (rear + 1) % BUFFER_SIZE;
}

int getkey(void)
{
    if (is_buffer_empty())
        return -1;

    int ret = buffer[front];
    front = (front + 1) % BUFFER_SIZE;
    size--;

    return ret;
}

void keyboard_handler(regs_t *regs)
{
    (void)regs;
    // check if the output buffer is full
    if (inb(KEY_STATUS) & 0x01)
    {
        u8 scancode = inb(KEY_BUFFER);
        if (scancode & 0x80)
            return;
        enqueue_key(scancode);
    }
}

void keyboard_init(void)
{
    irq_install_handler(KEY_IRQ, keyboard_handler);
}
