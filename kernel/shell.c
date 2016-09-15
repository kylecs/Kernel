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
  if(keycode > 0 && ch > 2 && key_index + 1 < 200 && ch != '\n') {
    terminal_write_next_char(ch);
    buffer[key_index] = ch;
    key_index++;
  }

  //reprint kernel> on new line
  if(ch == '\n') {
    terminal_write_next_char('\n');
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

void help_menu(char* args) {
  for(uint8_t i = 0; i < command_index; i++) {
    print(commands[i].command);
    print(" - ");
    println(commands[i].description);
  }
}

void pong(char* args) {
  println("PONG!");
}

void cls(char* args) {
  terminal_clear();
}

void echo(char* args) {
  if(!args){
    println("");
    return;
  }
  println(args);
}

void TODO(char* args) {
  println("- Figure out how to interface with the IDE controller");
  println("- Create a filesystem driver, probably ext2.");
  println("- Add a better memory allocator, including the reuse of memory");
}

void linebreak(char* args) {
  terminal_linebreak();
}

void command_color(char* args){
  int len = strlen(args);
  if(len != 2 || !args) {
    println("Incorrect use of color, provide 2 characters only.");
    println("ex: color FF");
    println("The first color is background.");
    println("The second color is foreground.");
    println("0 - black        9  - light blue");
    println("1 - blue         A - light green");
    println("2 - green        B - light cyan");
    println("3 - cyan         C - light red");
    println("4 - red          D - light magenta");
    println("5 - magenta      E - yellow");
    println("6 - brown        F - white");
    println("7 - light gray");
    println("8 - dark gray");
    return;
  }

  terminal_set_color(hexchar_to_decimal(args[0]), hexchar_to_decimal(args[1]));
  terminal_clear();
}

void shell_initialize() {
  print("Initializing shell...\t");
  memset(commands, sizeof(commands), 0);
  register_command("help", help_menu, "Displays this menu.");
  register_command("ping", pong, "Responds with PONG!");
  register_command("cls", cls, "Clears the terminal.");
  register_command("todo", TODO, "Prints the short term list of things to do.");
  register_command("echo", echo, "Print given string.");
  register_command("break", linebreak, "Print a red separating line.");
  register_command("color", command_color, "Set the terminal colors.");
  print("done");
  terminal_linebreak();
  shell_print_kernel();
}

void shell_print_kernel() {
  uint8_t color = terminal_get_color();
  terminal_set_color(terminal_get_background(), 0xC);
  print("kernel>");
  terminal_set_color_code(color);
}
typedef void func(char* parameters);


void shell_handle_command() {
  char* cmd_buf = (char*)kalloc(sizeof(char) * (key_index + 1));
  cmd_buf[key_index] = 0;
  memcpy(buffer, cmd_buf, key_index);
  int len = strlen(cmd_buf);
  shell_reset_buffer();

  int space_index = string_index_of_char(cmd_buf, ' ');
  char* command;
  char* parameters;

  if(space_index >= 0){
    command = substring(cmd_buf, 0, space_index);
    parameters = substring(cmd_buf, space_index + 1, len);
  }else{
    command = cmd_buf;
    parameters = 0;
  }

  //see if the command exists
  uint8_t handled = 0;
  for(int i = 0; i < command_index; i++){
    if(strcmp(command, commands[i].command)){
      ((func*)commands[i].callback)(parameters);
      handled = 1;
      break;
    }
  }
  if(!handled) {
    println("Command not recognized! Type 'help' for some commands.");
  }
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
