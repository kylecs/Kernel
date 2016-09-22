#include "include/ata.h"
#include "include/pci.h"
#include "include/io.h"

static ata_device_t ata_primary = {.io_base = 0x1F0, .control=0x3F6};
static ata_device_t ata_secondary = {.io_base = 0x170, .control=0x376};

void setup_ata(){
  int size = 0;
  pci_device_t* device = pci_get_devices(&size);
  printf("Scanning %s devices for IDE controllers.\n", size);
  for(int i = 0; i < size; i++){
    if(device[i].class == 0x01 && device[i].subclass == 0x1){
      println("PCI IDE Controller found!");
      setup_device(&device[i]);
      return;
    }
  }
}

void setup_device(pci_device_t* device){
  println("Setting up ATA");
  uint32_t result = pci_device_read(device, 0x14);
  printf("Bar1 result: %s\n", result);
}
