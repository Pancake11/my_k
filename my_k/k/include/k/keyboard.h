#pragma once

#define KEY_BUFFER 0x60
#define KEY_STATUS 0x64
#define KEY_IRQ 1

int getkey(void);
void keyboard_init(void);
