#ifndef H_IO
#define H_IO
#include <stdint.h>
#include <stddef.h>

extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);
extern void enable_interrupts();
extern void disable_interrupts();

#endif
