#include "include/terminal.h"
#include "include/io.h"
#include <stddef.h>
#include <stdint.h>
#include "include/string.h"

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

void kmain(uintptr_t stack_top,uintptr_t stack_bottom,multiboot_header_t* mboot, uint32_t magic){
	terminal_initialize();
	printf("Magic number: %s\n", magic);
	print_multiboot_info(mboot);
	printf("Stack bottom: %sB\n", stack_bottom);
	printf("Stack top: %sB\n", stack_top);
	printf("Size of uint64_t is %s\n", sizeof(uint64_t));
	printf("Print of -11 is %s\n", -11);
}
