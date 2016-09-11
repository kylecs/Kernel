#include "include/keyboard.h"
#include "include/io.h"
#include "include/terminal.h"
#include "include/interrupts.h"
#include "include/shell.h"

//#define KEYBOARD_PRINTCODE

uint8_t shift = 0;
uint8_t caps = 0;

void keyboard_interrupt_handler() {
  uint8_t status;
	signed char keycode;
	//write interrupt end
	outb(0x20, 0x20);

	status = inb(0x64);
	if(status & 1){
		keycode = inb(0x60);
    handle_key(keycode);
    #ifdef KEYBOARD_PRINTCODE
    printf("%s\n", keycode);
    #endif
	}
}

void handle_key(int32_t keycode) {
  switch(keycode) {
    case SHIFT:
      shift = 1;
      break;
    case SHIFT_RELEASE:
      shift = 0;
      break;
    case CAPSLOCK:
      if(caps > 0){
        caps = 0;
      }else {
        caps = 1;
      }
      break;
    default:
      if(keycode < 0) return;
      char ch;
      if(shift || caps) {
        ch = keymap_upper[keycode];
      }else {
        ch = keymap_lower[keycode];
      }
      //case handled, now pass to shell
      shell_handle_key(keycode, ch);

  }
}

void install_keyboard() {
  print("Installing keyboard ... ");
  install_interrupt_handler(33, keyboard_interrupt_handler);
  print("done\n");
}
