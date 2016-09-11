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
	terminal_linebreak();
	println("Beginning boot sequence;");
	install_gdt();
	install_idt();
	install_interrupt_interface();
	install_keyboard();
	shell_initialize();
	while(1) {}
}
