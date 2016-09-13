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
	uprintf("Checking PCI address: %s\n", address);
	outdw(0xCF8, address);
	uint32_t got = indw(0xCFC);
	return got;
}

void scan_pci() {
	//TODO
	/*
	uint16_t vendor_id = (got & 0xFFFF);
	uint16_t device_id = (got >> 16) & 0xFFFF;
	println("");
	uprintf("Got PCI response: %s\n", got);
	uprintf("Vendor ID: %s\n", vendor_id);
	uprintf("Device ID: %s\n", device_id);*/
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
	println("");
	printsf("The value of 112 in hex is %s\n", int_to_hexstring(112));
	while(1) {}
}
