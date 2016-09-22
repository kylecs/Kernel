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
#include "include/ata.h"

void kmain(uintptr_t stack_top, uintptr_t stack_bottom,
						multiboot_header_t* mboot, uint32_t magic){
	terminal_initialize();
	terminal_linebreak();
	//print some non-multiboot table things
	printf("Magic number:\t%s\n", magic);
	printsf("Stack bottom:\t%s\n", uint_to_hexstring(stack_bottom));
	printsf("Stack top:\t%s\n", uint_to_hexstring(stack_top));


	terminal_linebreak();
	//dive into multiboot data structure data
	printsf("Flags:\t\t%s\n", uint_to_binstring(mboot->flags));
	printsf("Bootloader:\t%s\n", mboot->boot_loader_name);
	printsf("Lower memory:\t", uint_to_hexstring(mboot->mem_lower));
	printsf("Upper memory:\t", uint_to_hexstring(mboot->mem_upper));
	printsf("Mods count:\t%s\n", uint_to_string(mboot->mods_count));
	printsf("Mods addr:\t%s\n", uint_to_hexstring(mboot->mods_addr));
	terminal_linebreak();
	install_gdt();
	install_idt();
	install_interrupt_interface();
	pci_scan();
	setup_ata();
	install_keyboard();

	shell_initialize();
	register_command("pci_scan", pci_print_all, "Scan PCI for devices.");
	while(1) {}
}
