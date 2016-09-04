#ifndef IDT_H
#define IDT_H

#include "terminal.h"
#include "io.h"

#define PIC1_CONTROL 0x20
#define PIC1_DATA 0x21
#define PIC2_CONTROL 0xA0
#define PIC2_DATA 0xA1

#define NUM_IDT_GATES 256

struct __attribute__((__packed__)) idt_gate{
  uint16_t offset_lower;
  uint16_t selector;
  uint8_t _empty;
  uint8_t flags;
  uint16_t offset_upper;
};
typedef struct idt_gate idt_gate_t;

struct __attribute__((__packed__)) idt_descriptor{
  uint16_t limit;
  uint32_t base;
};
typedef struct idt_descriptor idt_descriptor_t;

extern void load_idt(idt_descriptor_t idtr);

void install_idt();
void add_idt_entry(uint16_t num, unsigned long offset, uint16_t selector, uint8_t flags);


#endif
