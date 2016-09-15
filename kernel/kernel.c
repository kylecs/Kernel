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
#include "include/pci.h"

void kmain(uintptr_t stack_top, uintptr_t stack_bottom,
						multiboot_header_t* mboot, uint32_t magic){
	terminal_initialize();
	terminal_linebreak();
	//print some non-multiboot table things
	printf("Magic number:\t%s\n", magic);
	printf("Stack bottom:\t%sB\n", stack_bottom);
	printf("Stack top:\t%sB\n", stack_top);

	terminal_linebreak();
	//dive into multiboot data structure data
	printf("Flags:\t\t%s\n", mboot->flags);
	print("Bootloader: \t");	println(mboot->boot_loader_name);
	printf("Lower memory:\t%sKB\n", mboot->mem_lower);
	printf("Upper memory:\t%sKB\n", mboot->mem_upper);
	printf("Module count:\t%s\n", mboot->mods_count);
	printf("Modules addr:\t%s\n", mboot->mods_addr);
	terminal_linebreak();
	install_gdt();
	install_idt();
	install_interrupt_interface();
	pci_scan();
	install_keyboard();
	shell_initialize();
	register_command("pci_scan", pci_print_all, "Scan PCI for devices.");
	while(1) {}
}
