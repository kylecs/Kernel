#ifndef H_TERMINAL
#define H_TERMINAL
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "string.h"

#define TERMINAL_COMMAND_PORT 0x3D4
#define TERMINAL_DATA_PORT 0x3D5

struct vga_entry{
	char character;
	uint8_t color;
	uint16_t data;
};
typedef struct vga_entry vga_entry;

//function definitions
void terminal_writeline(char* c);
void terminal_clear();
void terminal_scroll();
void terminal_initialize();
void terminal_backspace();
void terminal_write_next_entry(vga_entry entry);
void terminal_write_next_char(char c);
vga_entry terminal_make_vga_entry(char c);
uint8_t terminal_make_color(uint8_t foreground, uint8_t background);
void terminal_write_char_at(uint16_t x, uint16_t y, vga_entry entry);
void terminal_set_cursor_position(uint8_t x, uint8_t y);
void printf(char* str, int32_t rpl);
void print(char* str);
uint8_t terminal_getX();
uint8_t terminal_getY();
void terminal_set_color(uint8_t forground, uint8_t background);
#endif
