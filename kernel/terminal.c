#include "include/terminal.h"

uint16_t* terminal = (uint16_t*)0xB8000;
uint8_t color = 0;
vga_entry whitespace;
uint8_t x, y;
const uint16_t WIDTH = 80;
const uint16_t HEIGHT = 25;
uint8_t terminal_make_color(uint8_t foreground, uint8_t background){
	return foreground | background << 4;
}

vga_entry terminal_make_vga_entry(char c){
	vga_entry ret;
	ret.character = c;
	ret.color = color;
	ret.data = c | color << 8;
	return ret;

}
void terminal_write_char_at(uint16_t x, uint16_t y, vga_entry entry){
	//if(x == WIDTH ) return;
	//if(y == HEIGHT) return;
	terminal[y * WIDTH + x] = entry.data;
}
void terminal_set_cursor_position(uint8_t x, uint8_t y){
	uint16_t pos = y*WIDTH + x;
	outw(TERMINAL_COMMAND_PORT, 14);
	outw(TERMINAL_DATA_PORT, (pos >> 8) & 0x00FF);
	outw(TERMINAL_COMMAND_PORT, 15);
	outw(TERMINAL_DATA_PORT, pos & 0x00FF);


}
void terminal_write_next_entry(vga_entry entry){
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
	for(uint16_t n = 0; n < WIDTH * HEIGHT; n++){
		terminal[n] = whitespace.data;
	}
}
void terminal_initialize(){
	x = 0;
	y = 0;
	color = terminal_make_color(15, 0);
	whitespace = terminal_make_vga_entry(' ');
	terminal_clear();
	print("Initializing terminal...\n");


}
void terminal_write_next_char(char c){
	terminal_write_next_entry(terminal_make_vga_entry(c));
}

void terminal_writeline(char* string){
	uint16_t length = strlen(string);
	for(uint16_t i = 0; i < length; i++){
		terminal_write_next_entry(terminal_make_vga_entry(string[i]));
	}
}
void terminal_scroll(){
	for(uint16_t i = 0; i < 80*24; i++){
		terminal[i] = terminal[i + 80];
	}
	for(uint16_t i = 80*24; i < 80*25; i++){
		terminal[i] = whitespace.data;
	}
}
void printf(char* str, int32_t rpl){
	if(rpl == NULL){
		print(str);
		return;
	}
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
