#pragma once
#include "types.h"

#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

#define MAX_PCI_FUNCTIONS 16

static char* const DEVICE_CLASS_LIST[] = {
	"Unclassified device",
	"Mass storage controller",
	"Network controller",
	"Display controller",
	"Multimedia controller",
	"Memory controller",
	"Bridge",
	"Communication controller",
	"Generic system peripheral",
	"Input device controller",
	"Docking station",
	"Processor",
	"Serial bus controller",
	"Wireless controller",
	"Intelligent controller",
	"Satellite communications controller",
	"Encreption controller",
	"Signal processing controller",
	"Processing accelerators",
	"Non-essential instrumentation", //offset 0x13
};

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
pci_device_t* pci_get_devices(int* size);
void pci_device_write(pci_device_t* device, uint8_t offset, uint32_t);
uint32_t pci_device_read(pci_device_t* device, uint8_t offset);
