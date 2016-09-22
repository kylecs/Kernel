#pragma once
#include "types.h"
#include "pci.h"

//Ata status port results
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Inlex
#define ATA_SR_ERR     0x01    // Error

typedef struct {
  int io_base;
  int control;
} ata_device_t;

void setup_ata();
void setup_device(pci_device_t* device);
