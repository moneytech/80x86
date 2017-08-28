#include "bios.h"
#include "bda.h"
#include "io.h"
#include "serial.h"
#include "timer.h"

static void timer_services(struct callregs *regs)
{
    switch (regs->ax.h) {
    case 0x0:
        regs->dx.x = bda_read(timer_counter_low);
        regs->cx.x = bda_read(timer_counter_high);
        regs->flags &= ~CF;
        break;
    case 0x1:
        bda_write(timer_counter_low, regs->dx.x);
        bda_write(timer_counter_high, regs->cx.x);
        regs->flags &= ~CF;
        break;
    default:
        regs->flags |= CF;
    }
}
VECTOR(0x1a, timer_services);

void init_timer(void)
{
    outb(0xfff5, 8);
    outb(0xfff4, 1);
    outw(0xffee, (1 << 15) | 55);
}