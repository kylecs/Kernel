#include "include/ide.h"
#include "include/pci.h"

void scan_for_ide(){
  int pci_size = 0;
  pci_device_t* devices = pci_get_devices(&pci_size);
  for(int i = 0; i < pci_size; i++){
    if(devices[i].class == 0x01 && devices[i].subclass == 0x1){
      println("IDE Controller Found!");
    }
  }
}
