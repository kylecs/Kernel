#include "include/shell.h"
#include "include/terminal.h"
#include "include/keyboard.h"
#include "include/memory.h"
#include "include/string.h"

char buffer[200];
uint16_t key_index = 0;


void shell_handle_key(int32_t keycode, char ch) {
  //write to terminal if it has a character
  if(keycode > 0 && ch > 2) {
    terminal_write_next_char(ch);
    buffer[key_index] = ch;
    key_index++;
  }

  //reprint kernel> on new line
  if(ch == '\n') {
    shell_handle_command();
    shell_print_kernel();
  }
  //handle some special characters
  if(keycode == BACKSPACE) {
    //protect the 'kernel>' prefix
    if(terminal_getX() > 7) {
      terminal_backspace();
    }
  }
  if(keycode == ESCAPE) {
    terminal_clear();
    shell_print_kernel();
    shell_reset_buffer();
  }
}

void shell_initialize() {
  print("Initializing shell...\n");
  shell_print_kernel();
}

void shell_print_kernel() {
  terminal_set_color(0xC, 0x0);
  print("kernel>");
  terminal_set_color(0xF, 0x0);
}

void shell_handle_command() {
  char* c = "uname";
  if(string_starts_with(buffer, c)) {
    print("This is the hobby OS of Kyle Spencer!\n");
  }else if (buffer[0] == '\n') {

  }else {
    print("That is not a recognized command!\n");
    print("The recognized command is 'uname'\n");
  }
  //do something with buffer
  shell_reset_buffer();
}

void shell_reset_buffer() {
  key_index = 0;
  memset(buffer, sizeof(buffer), 0);
}
