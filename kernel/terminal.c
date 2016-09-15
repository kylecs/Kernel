#include "include/terminal.h"
#include "include/memory.h";

//some constants
const uint16_t WIDTH = 80;
const uint16_t HEIGHT = 25;

//terminal global variables
vga_entry_t* terminal = (vga_entry_t*)0xB8000;
uint8_t color = 0;
uint8_t background = 0;
uint8_t foreground = 0;
uint8_t x, y;

//store history and future for scrolling support
vga_entry_t* history;
uint32_t history_index = 0;

vga_entry_t* future;
uint32_t future_index = 0;

uint8_t terminal_make_color(uint8_t background, uint8_t foreground){
	return foreground | background << 4;
}

vga_entry_t terminal_make_vga_entry(char c){
	vga_entry_t ret;
	ret.character = c;
	ret.color = color;
	return ret;
}

void terminal_set_color(uint8_t bg, uint8_t fg) {
	background = bg;
	foreground = fg;
	color = terminal_make_color(bg, fg);
}

uint8_t terminal_get_background(){
	return background;
}

uint8_t terminal_get_foreground(){
	return foreground;
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
	terminal_write_char_at(x, y, terminal_make_vga_entry(' '));
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
	}else if(entry.character == '\t'){
		while(x % 7 != 0){
			x++;
			if(x >= 80){
				x = 0;
				y++;
				if(y > 25){
					terminal_scroll();
				}
			}
		}
	}else{
		terminal_write_char_at(x, y, entry);
		x++;
	}
	terminal_set_cursor_position(x, y);

}

void terminal_clear(){
	terminal_write_next_char('\n');
	for(int i = 0; i < 25 * 80; i++){
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
		terminal[n] = terminal_make_vga_entry(' ');
	}
	x = 0, y = 0;
	terminal_set_cursor_position(x, y);
}

void terminal_initialize(){
	//configure vga to use last bit of vga_entry as color and not a blink bit
	inb(0x3DA); //reset VGA_CONTROL to access an index next
	uint8_t state = inb(VGA_CONTROL);
	outb(VGA_CONTROL, 0x10);
	uint8_t attr = inb(VGA_CONTROL_READ);
	attr &= 0xF7;
	outb(VGA_CONTROL, attr);
	outb(VGA_CONTROL, state);
	inb(0x3DA);

	x = 0;
	y = 0;
	color = terminal_make_color(0, 15);
	terminal_hard_clear();
	print("Initializing terminal...\n");
	history = (vga_entry_t*)kalloc(sizeof(char) * 32000);
	future = (vga_entry_t*)kalloc(sizeof(char) * 32000);
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
	if(history_index + 80 >= 32000){
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
			terminal[i] = terminal_make_vga_entry(' ');
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
	printsf(str, int_to_string(rpl));
}

void uprintf(char* str, uint32_t rpl){
	printsf(str, uint_to_string(rpl));
}

void printsf(char* str, char* repl){
	uint16_t size = strlen(str);
	for(uint16_t i = 0; i < size - 1; i++){
		if(str[i] == '%' && str[i+1] == 's'){
			char* str1 = substring(str, 0, i);
			char* str2 = substring(str, i+2, size);
			print(str1);
			print(repl);
			print(str2);
			return;
		}
	}
}
void print(char* str){
	terminal_writeline(str);
}

void println(char* str){
	print(str);
	print("\n");
}

uint8_t terminal_get_color(){
	return color;
}

void terminal_set_color_code(uint8_t newcolor){
	color = newcolor;
	background = (0xF0 & color) >> 4;
	foreground = 0xF & color;
}

void terminal_linebreak(){
	if(x > 0){
		print("\n");
	}
	uint8_t prev_color = color;
	terminal_set_color(background, 0xC);
	for(uint8_t i = 0; i < 80; i++) {
		print("-");
	}
	color = prev_color;
}
