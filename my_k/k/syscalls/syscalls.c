#include <k/idt.h>
#include <k/irq.h>
#include <k/kstd.h>
#include <k/sys.h>
#include <k/timer.h>
#include <k/types.h>
#include <stdio.h>

#define MAX_SYSCALL

typedef int syscall(int ebx, int ecx, int edx);

syscall *dispatch_table[MAX_SYSCALL];
static int size = 0;

int test(int ebx, int ecx, int edx)
{
    printf("Test\n");
    return 0;
}

int null(int ebx, int ecx, int edx)
{
    return 0;
}

static inline void do_syscalls(regs_t *regs)
{
    u8 nbr = regs->eax; // we only need the low bytes for the syscalls

    regs->eax = dispatch_table[nbr](regs->ebx, regs->ecx, regs->edx);
}

void syscall_add(int i, syscall handler)
{
    dispatch_table[i] = handler;
    size++;
}

void syscall_handler(regs_t *regs)
{
    do_syscalls(regs);
}

void syscall_init(void)
{
    syscall_add(0,null);
    syscall_add(1,test);
}
