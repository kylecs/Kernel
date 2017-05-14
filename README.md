# Kernel
A simple x86 kernel written in C and Assembly

## Demo
*Click to play*

[![Demo Video HERE](https://img.youtube.com/vi/McJ6sdyONg8/0.jpg)](https://www.youtube.com/watch?v=McJ6sdyONg8)

## Features
  * Color Terminal with scrolling(page up and down keys)
  * Scan PCI devices and print some info about them
  * Prints multiboot data
  * Basic command(cls, echo, help, color)

## Install
  *You must have a GCC cross compiler targeting i686-elf.*
  1. Modify the make file to point $(CC) and $(AS) at your cross compiler.
  2. Run `make` to compile into an elf binary.
  3. Run `make run` to load the binary with qemu.
  4. Run `make buildiso` to make a bootable iso with grub-mkrescue.
