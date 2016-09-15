#include "include/terminal.h"
#include "include/io.h"
#include <stddef.h>
#include <stdint.h>
#include "include/string.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/interrupts.h"
#include "include/keyboard.h"
#include "include/shell.h"
#include "include/multiboot.h"

//BAR = base address registers
uint32_t query_pci(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
	uint32_t address = (1 << 31) | (bus << 16) | ((device & 0x1f) << 11) |
		((function & 0x7) << 8) | ((reg & 0x3F) << 2);
	outdw(0xCF8, address);
	uint32_t got = indw(0xCFC);
	return got;
}

void scan_pci(char* args) {
	uint8_t bus = 0;
	uint8_t device = 0;
	uint8_t continue_scan = 1;
	while(continue_scan){
		uint32_t result = query_pci(bus, device, 0, 0);
		if(result == 0xFFFFFFFF){
			if(device == 0){
				//out of devices
				continue_scan = 0;
			}else{
				//reached end of bus
				device = 0;
				bus++;
			}
		}else{
			uprintf("BUS: %s, ", bus);
			uprintf("DEVICE: %s\n", device);
			uint16_t vendor_id = result & 0xFFFF;
			uint16_t device_id = (result >> 16) & 0xFFFF;
			printsf("VENDORID: %s ", uint_to_hexstring(vendor_id));
			printsf("DEVICEID: %s\n", uint_to_hexstring(device_id));
			uint32_t reg_data = query_pci(bus, device, 0, 0x08);
			uint8_t class = (reg_data >> 24) & 0xFF;
			uint8_t subclass = (reg_data >> 16) & 0xFF;
			printsf("register 0x08: %s, ", uint_to_binstring(reg_data));
			printsf("CLASS: %s, ", uint_to_hexstring(class));
			printsf("SUBCLASS: %s", uint_to_hexstring(subclass));
			print("\n\n");
			device++;
		}
	}
	println("End PCI Scan");
}

void kmain(uintptr_t stack_top, uintptr_t stack_bottom,
						multiboot_header_t* mboot, uint32_t magic){
	terminal_initialize();
	terminal_linebreak();
	//print some non-multiboot table things
	printf("Magic number: %s\n", magic);
	printf("Stack bottom: %sB\n", stack_bottom);
	printf("Stack top: %sB\n", stack_top);

	terminal_linebreak();
	//dive into multiboot data structure data
	printf("Flags is %s\n", mboot->flags);
	print("Bootloader: ");	println(mboot->boot_loader_name);
	printf("Lower memory is %sKB\n", mboot->mem_lower);
	printf("Upper memory is %sKB\n", mboot->mem_upper);
	printf("Drives datastructure size: %s\n", mboot->drives_length);
	printf("Drives datastructure address: %s\n", mboot->drives_addr);
	printf("Module count: %s\n", mboot->mods_count);
	printf("Modules address: %s\n", mboot->mods_addr);
	terminal_linebreak();
	println("Beginning boot sequence;");
	install_gdt();
	install_idt();
	install_interrupt_interface();
	install_keyboard();
	shell_initialize();
	register_command("pci_scan", scan_pci, "Scan PCI for devices.");
	while(1) {}
}
