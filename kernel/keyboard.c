#include "include/keyboard.h"
#include "include/io.h"
#include "include/terminal.h"
#include "include/interrupts.h"

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
    //printf("Keycode: %s\n", keycode);
    handle_key(keycode);
	}
}

void handle_key(int32_t keycode) {
  //printf("Keycode: %s\n", keycode);
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
    case BACKSPACE:
      terminal_backspace();
      break;
    default:
      if(keycode < 0) return;
      if(shift || caps) {
        char str[2];
        str[1] = 0;
        str[0] = keymap_upper[keycode];
        print(&str);
      }else {
        char str[2];
        str[1] = 0;
        str[0] = keymap_lower[keycode];
        print(&str);
      }
  }
}

void install_keyboard() {
  print("Installing keyboard ... ");
  install_interrupt_handler(33, keyboard_interrupt_handler);
  print("done\n");
}
