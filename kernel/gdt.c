#include "include/gdt.h"

gdt_entry_t GDT[3];
gdt_descriptor_t gdt_desc;

void install_gdt() {
  print("Installing GDT ...");

  gdt_desc.offset = GDT;
  gdt_desc.size = sizeof(GDT) - 1;
  //empty entry
  memset(&(GDT[0]), sizeof(gdt_entry_t), 0);
  //code segment
  add_gdt_entry(1, 0, 0xFFFFFFFF, 1, 1, 1, 0, 1, 0, 1);
  //data segment
  add_gdt_entry(2, 0, 0xFFFFFFFF, 1, 1, 1, 0, 0, 0, 1);

  //debug data
  /*printf("GDT is located at %s\n", (uintptr_t)&(GDT[0]));
  printf("GDT Descriptor is located at %s\n", &gdt_desc);
  printf("Size of GDT - 1 is %s\n", sizeof(GDT) - 1);
  printf("Size of gdt_entry_t is %s\n", sizeof(gdt_entry_t));
  printf("gdt descriptor offset value is %s\n", gdt_desc.offset);*/

  //load it
  load_gdt(gdt_desc);
  print("done\n");

}

void add_gdt_entry(uint16_t offset, uint64_t base, uint64_t limit,
                    uint8_t size, uint8_t granularity, uint8_t readwrite,
                    uint8_t direction, uint8_t executable, uint8_t privilege,
                    uint8_t present)
{
  //encode base
  GDT[offset].base_lower = base & 0xFFFF; //that is the first 16 bits
  GDT[offset].base_middle = (base & 0xFF0000) >> 16;//8 bits after first 16
  GDT[offset].base_upper = (base & 0xFF000000) >> 24;//upper 8 bits

  //encode limit
  GDT[offset].limit_lower = limit & 0xFFFF; //first 16
  GDT[offset].limit_upper = (limit & 0xF0000) >> 16; //next 4

  //encode flags
  GDT[offset].size = size;
  GDT[offset].granulatity = granularity;

  //encode access byte
  GDT[offset].access = 0;
  GDT[offset].readwrite = readwrite;
  GDT[offset].direction = direction;
  GDT[offset].executable = executable;
  GDT[offset].privilege = privilege;
  GDT[offset].present = present;

  //some definate stuff
  GDT[offset].zero = 0;
  GDT[offset].one = 1;
}
