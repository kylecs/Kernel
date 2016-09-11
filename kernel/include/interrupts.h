#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stddef.h>
#include <stdint.h>


typedef struct {
  int present: 1;
  void* handler;
} interrupt_handler_t;

void install_interrupt_interface();
void general_interrupt_handler(int offset);

#endif
