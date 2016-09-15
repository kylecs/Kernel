#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stddef.h>

#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

#define MAX_PCI_FUNCTIONS 16

typedef struct {
  uint8_t bus;
  uint8_t device;
  uint8_t function;
  uint32_t address;
  uint16_t vendor_id;
  uint16_t device_id;
  uint8_t class;
  uint8_t subclass;
  uint8_t header_type;
} pci_device_t;

uint32_t pci_query(uint32_t address);
void pci_scan_bus(uint8_t bus);
void pci_scan();
uint32_t pci_make_address(uint8_t bus, uint8_t device, uint8_t function,
													uint8_t reg);
void pci_print_device(pci_device_t device);
void pci_print_all(char* args);



#endif
