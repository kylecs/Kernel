#pragma once
#include "types.h"

typedef struct command {
  uint8_t present;
  char* command;
  void* callback;
  char* description;
} command_t;

void shell_handle_key(int32_t keycode, char ch);
void shell_initialize();
void shell_print_kernel();
void shell_handle_command();
void shell_reset_buffer();
void register_command(char* command, void* callback, char* description);
void help_menu();
void pong();
