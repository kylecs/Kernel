#include "include/io.h"


void outb(uint16_t port, uint8_t data){

	asm("out %1,  %0" : : "dN"(port), "a"(data));
}


uint8_t inb(uint16_t port){
	uint8_t ret = 0;
	asm("in %0" : "=r"(port) : "r"(ret));
	return ret;
}
