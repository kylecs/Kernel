#include "include/terminal.h"
#include "include/memory.h";

//some constants
const uint16_t WIDTH = 80;
const uint16_t HEIGHT = 25;

//terminal global variables
vga_entry_t* terminal = (vga_entry_t*)0xB8000;
uint8_t color = 0;
vga_entry_t whitespace;
uint8_t x, y;

//store history and future for scrolling support
vga_entry_t* history;
uint32_t history_index = 0;

vga_entry_t* future;
uint32_t future_index = 0;

uint8_t terminal_make_color(uint8_t foreground, uint8_t background){
	return foreground | background << 4;
}

vga_entry_t terminal_make_vga_entry(char c){
	vga_entry_t ret;
	ret.character = c;
	ret.color = color;
	return ret;
}

void terminal_set_color(uint8_t foreground, uint8_t background) {
	color = terminal_make_color(foreground, background);
}

void terminal_write_char_at(uint16_t x, uint16_t y, vga_entry_t entry){
	terminal[y * WIDTH + x] = entry;
}
void terminal_set_cursor_position(uint8_t x, uint8_t y){
	uint16_t pos = y*WIDTH + x;
	outb(TERMINAL_COMMAND_PORT, 14);
	outb(TERMINAL_DATA_PORT, (pos >> 8) & 0x00FF);
	outb(TERMINAL_COMMAND_PORT, 15);
	outb(TERMINAL_DATA_PORT, pos & 0x00FF);
}

uint8_t terminal_getX() {
	return x;
}

uint8_t terminal_getY() {
	return y;
}

void terminal_backspace(){
	terminal_reset_scroll();
	if(x > 0) x--;
	terminal_write_char_at(x, y, whitespace);
	terminal_set_cursor_position(x, y);
}


void terminal_write_next_entry(vga_entry_t entry){
	if(x == WIDTH){
		x = 0;
		y++;
	}
	if(y == HEIGHT){
		terminal_scroll();
	}
	if(entry.character == '\n'){
		x = 0;
		y++;
	}else{
		terminal_write_char_at(x, y, entry);
		x++;
	}
	terminal_set_cursor_position(x, y);

}

void terminal_clear(){
	terminal_write_next_char('\n');
	for(int i = 0; i < 24 * 80; i++){
		terminal_write_next_char(' ');
	}
	x = 0;
	y = 0;
	terminal_set_cursor_position(x, y);
}

void terminal_hard_clear(){
	future_index = 0;
	history_index = 0;
	for(uint16_t n = 0; n < WIDTH * HEIGHT; n++){
		terminal[n] = whitespace;
	}
	x = 0, y = 0;
	terminal_set_cursor_position(x, y);
}

void terminal_initialize(){
	x = 0;
	y = 0;
	color = terminal_make_color(15, 0);
	whitespace = terminal_make_vga_entry(' ');
	terminal_hard_clear();
	print("Initializing terminal...\n");
	history = (vga_entry_t*)kalloc(8000);
	future = (vga_entry_t*)kalloc(8000);
}
void terminal_write_next_char(char c){
	terminal_reset_scroll();
	terminal_write_next_entry(terminal_make_vga_entry(c));
}

void terminal_writeline(char* string){
	terminal_reset_scroll();
	uint16_t length = strlen(string);
	for(uint16_t i = 0; i < length; i++){
		terminal_write_next_entry(terminal_make_vga_entry(string[i]));
	}
}

void terminal_reset_scroll() {
	while(future_index >= 80) {
		terminal_scroll();
	}
}

//only scrolls if there is some future to move into the current row
void terminal_replay_future(){
	if(future_index >= 80) {
		terminal_scroll();
	}

}

void terminal_scroll(){
	//don't allow scroll if the buffer can't hold any more data
	if(history_index + 80 >= 8000){
		return;
	}

	for(int i = 0; i < 80; i++){
		history[history_index] = terminal[i];
		history_index++;
		terminal[i] = terminal[i + 80];
	}

	for(uint16_t i = 80; i < 80*24; i++){
		terminal[i] = terminal[i + 80];
	}

	//fill bottom row with whitespace if future doesn't contain enough data
	if(future_index < 80) {
		for(uint16_t i = 80*24; i < 80*25; i++){
			terminal[i] = whitespace;
		}
		y--;
		x = 0;
	}else {
		for(uint16_t i = 80*25 - 1; i >= 80*24; i--){
			future_index--;
			terminal[i] = future[future_index];
		}
	}
	if(future_index < 80) {
		terminal_set_cursor_position(x, y);
	}
}

void terminal_up() {
	if(history_index < 80) {
		//no rows have been scrolled away
		return;
	}

	//place bottom row into future
	for(int i = 80*24; i < 80 * 25; i++) {
		future[future_index] = terminal[i];
		future_index++;
	}

	//move all middle rows, which are just shifted in the array
	for(int i = 80 * 25 - 1; i >= 80; i--) {
		terminal[i] = terminal[i - 80];
	}

	//insert first row from history
	for(int i = 80 - 1; i >= 0; i--) {
		history_index--;
		terminal[i] = history[history_index];
	}
	terminal_set_cursor_position(0xFF, 0xFF);
}

void printf(char* str, int32_t rpl){
	uint16_t size = strlen(str);
	for(uint16_t i = 0; i < size - 1; i++){
		if(str[i] == '%' && str[i + 1] == 's'){
			char* str1 = substring(str,0, i);
			char* str2 = int_to_string(rpl);

			char* str3 = substring(str, i + 2, size);
			char* p1 = concat(str1, str2);
			char* p2 = concat(p1, str3);
			print(p2);
			return;
		}
	}
	print(str);
}
void print(char* str){
	terminal_writeline(str);
}

void println(char* str){
	print(str);
	print("\n");
}

void terminal_linebreak(){
	if(x > 0){
		print("\n");
	}
	uint8_t prev_color = color;
	terminal_set_color(0xC, 0x0);
	for(uint8_t i = 0; i < 80; i++) {
		print("-");
	}
	color = prev_color;
}
