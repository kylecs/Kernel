#ifndef H_IO
#define H_IO
#include <stdint.h>
#include <stddef.h>

extern void outw(uint16_t port, uint16_t data);
extern uint16_t inw(uint16_t port);

#endif
