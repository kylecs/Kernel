#include "include/shell.h"
#include "include/terminal.h"
#include "include/keyboard.h"
#include "include/memory.h"
#include "include/string.h"

char buffer[200];
uint16_t key_index = 0;

command_t commands[10];
uint8_t command_index = 0;

void shell_handle_key(int32_t keycode, char ch) {
  //write to terminal if it has a character
  if(keycode > 0 && ch > 2 && key_index + 1 < 200) {
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
      //backspace command buffer as well
      buffer[key_index] = 0;
      key_index--;
    }
  }

  if(keycode == ESCAPE) {
    terminal_clear();
    shell_print_kernel();
    shell_reset_buffer();
  }

  if(keycode == PAGEUP) {
    terminal_up();
  }

  if(keycode == PAGEDOWN) {
    terminal_replay_future();
  }
}

void help_menu() {
  for(uint8_t i = 0; i < command_index; i++) {
    print(commands[i].command);
    print(" - ");
    println(commands[i].description);
  }
}

void pong() {
  println("PONG!");
}

void TODO() {
  println("- Move old print statements ending with NEWLINE to the" \
    " new println function");
  println("- Create an arraylist so dynamic memory can be used properly.");
  println("- Add parameters to functions that can be passed to their handler" \
  " functions");
  println("- Create a filesystem driver, probably ext2.");
  println("- Enable paging");
  println("- Add a better memory allocator, including the reuse of memory");
}

void shell_initialize() {
  println("Initializing shell...");
  memset(commands, sizeof(commands), 0);
  register_command("help", help_menu, "Displays this menu.");
  register_command("ping", pong, "Responds with PONG!");
  register_command("cls", terminal_clear, "Clears the terminal.");
  register_command("todo", TODO, "Prints the short term list of things to do.");
  terminal_linebreak();
  shell_print_kernel();
}

void shell_print_kernel() {
  terminal_set_color(0xC, 0x0);
  print("kernel>");
  terminal_set_color(0xF, 0x0);
}
typedef void func(void);

void shell_handle_command() {
  uint8_t handled = 0;
  for(int i = 0; i < command_index; i++){
    if(string_starts_with(buffer, commands[i].command)){
      ((func*)commands[i].callback)();
      handled = 1;
      break;
    }
  }
  if(!handled) {
    println("Command not recognized! Type 'help' for some commands.");
  }
  //do something with buffer
  shell_reset_buffer();
}

void shell_reset_buffer() {
  key_index = 0;
  memset(buffer, sizeof(buffer), 0);
}

void register_command(char* command, void* callback, char* description) {
  command_t newcommand;
  newcommand.command = command;
  newcommand.callback = callback;
  newcommand.description = description;
  commands[command_index] = newcommand;
  command_index++;
}
