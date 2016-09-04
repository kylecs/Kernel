#include "include/terminal.h"
#include "include/io.h"
#include <stddef.h>
#include <stdint.h>
#include "include/string.h"
#include "include/gdt.h"
#include "include/idt.h"

struct multiboot_header{
	uintptr_t flags;
	uintptr_t mem_lower;
	uintptr_t mem_upper;

};
typedef struct multiboot_header multiboot_header_t;

void print_multiboot_info(multiboot_header_t* mboot){
	printf("Flags is %s\n", mboot->flags);
	printf("Lower memory is %sKB\n", mboot->mem_lower);
	printf("Upper memory is %sKB\n", mboot->mem_upper);
}


//TODO Make some sort of enum for this?
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void nevercalled(){
	print("This should never be called!");
}


void kmain(uintptr_t stack_top,uintptr_t stack_bottom,multiboot_header_t* mboot, uint32_t magic){
	terminal_initialize();
	printf("Magic number: %s\n", magic);
	print_multiboot_info(mboot);
	printf("Stack bottom: %sB\n", stack_bottom);
	printf("Stack top: %sB\n", stack_top);
	install_gdt();
	install_idt();
	print("You may now type, no backspace or shift modifiers supported.\n");
	print("Also page scrolling doesn't seem to work atm. TODO\n:");
	while(1) {}
}

void c_handle_interrupt(){
	uint8_t status;
	char keycode;

	//write interrupt end
	outb(0x20, 0x20);

	status = inb(0x64);
	if(status & 1){
		keycode = inb(0x60);
		if(keycode > 0) {
			char* str[2];
			str[1] = 0;
			str[0] = kbdus[keycode];
			print(&str);
		}
	}
}

void c_gpf_handler(){
	print("GENERAL PROTECTION FAULT");
}
