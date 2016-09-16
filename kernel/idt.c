#include "include/idt.h"
#include "include/io.h"
#include "include/terminal.h"
#include "include/io.h"

extern void handle_interrupt(void);
extern void gpf_handler(void);

idt_gate_t IDT[NUM_IDT_GATES];
idt_descriptor_t idt_desc;

void install_idt() {
  print("Installing IDT ...\t\t");
  size_t size = sizeof(idt_gate_t) * NUM_IDT_GATES;
  idt_descriptor_t idt_desc;
  idt_desc.base = IDT;
  idt_desc.limit = sizeof(IDT) - 1;

  //configure PIC
  outb(PIC1_CONTROL, 0x11);
  outb(PIC2_CONTROL, 0x11);

  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28);

  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);

  outb(PIC1_DATA, 0x01);
  outb(PIC2_DATA, 0x01);

  //only enable keyboard IRQ
  outb(PIC1_DATA, 0b11111101);
  outb(PIC2_DATA, 0xff);

  load_idt(idt_desc);
  print("done\n");

  enable_interrupts();
}

void add_idt_entry(uint16_t num, unsigned long offset, uint16_t selector, uint8_t flags) {
  IDT[num].offset_lower = offset & 0xFFFF;
  IDT[num].offset_upper = (offset & 0xFFFF0000) >> 16;

  IDT[num].selector = selector;

  IDT[num]._empty = 0;

  IDT[num].flags = flags;
}
