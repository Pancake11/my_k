#pragma once

#include <k/sys.h>

void isr_init();
void isr_handler(regs_t *regs);
