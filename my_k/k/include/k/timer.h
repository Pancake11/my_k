#pragma once

#define CHANNEL0 0x40
#define CHANNEL1 0x41
#define CHANNEL2 0x42

#define CONTROL 0x43

#define TIMER_IRQ 0

#define BASE_FREQ 1193182
#define FREQ 100

void timer_init(void);
unsigned long gettick(void);
