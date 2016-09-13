#ifndef H_IO
#define H_IO
#include <stdint.h>
#include <stddef.h>

extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);

extern void outw(uint16_t port, uint16_t data);
extern uint16_t inw(uint16_t port);

extern void outdw(uint16_t port, uint32_t data);
extern uint32_t indw(uint16_t port);

extern void enable_interrupts();
extern void disable_interrupts();

#endif
