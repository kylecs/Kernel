#ifndef GDT_H
#define GDT_H
#include <stdint.h>
#include <stddef.h>
#include "terminal.h"
#include "memory.h"
#include "io.h"

typedef struct {
  uint16_t limit_lower;
  uint16_t base_lower;
  uint8_t base_middle;

  //access byte
  int access: 1;
  int readwrite: 1;
  int direction: 1;
  int executable: 1;
  int one: 1;
  int privilege: 2;
  int present: 1;

  int limit_upper : 4;

  //flags
  int zero: 2;
  int size: 1;
  int granulatity: 1;

  uint8_t base_upper;
} __attribute__((__packed__)) gdt_entry_t;

typedef struct {
  uint16_t size;
  uintptr_t offset;
}  __attribute__((__packed__)) gdt_descriptor_t;

extern void load_gdt(gdt_descriptor_t gdtr);

void install_gdt();

void add_gdt_entry(uint16_t offset, uint64_t base, uint64_t limit,
                    uint8_t size, uint8_t granularity, uint8_t readwrite,
                    uint8_t direction, uint8_t executable, uint8_t privlege,
                    uint8_t present);
#endif
