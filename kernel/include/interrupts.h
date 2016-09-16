#pragma once
#include "types.h"


typedef struct {
  int present: 1;
  void* handler;
} interrupt_handler_t;

void install_interrupt_interface();
void general_interrupt_handler(int offset);
