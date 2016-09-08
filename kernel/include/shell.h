#ifndef SHELL_H
#define SHELL_H
#include <stdint.h>
void shell_handle_key(int32_t keycode, char ch);
void shell_initialize();
void shell_print_kernel();
void shell_handle_command();
void shell_reset_buffer();
#endif
