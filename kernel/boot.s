.intel_syntax noprefix

/*Aligns boot modules on 4KB boundries*/
.set ALIGN, 1<<0

/*Requires memory data in the multiboot structure*/
.set MEMINFO, 1 << 1

/*Requires video mode info be available to the kernel*/
.set VIDEOINFO, 1<<2

/*
.set VIDEOMODE, 1
.set VIDEOWIDTH, 200
.set VIDEOHEIGHT, 30
.set VIDEODEPTH, 0
*/

.set MAGIC, 0x1BADB002
.set FLAGS, ALIGN | MEMINFO /*| VIDEOINFO*/
.set CHECKSUM, -(MAGIC + FLAGS)


.section .multiboot
  .align 4
  .long MAGIC
  .long FLAGS
  .long CHECKSUM
  .skip 20
  /*
  .long VIDEOMODE
  .long VIDEOWIDTH
  .long VIDEOHEIGHT
  .long VIDEODEPTH
  */

/*
a and w are both attributes, a means allocatable and w means writable
@nobits means that the section does not contain data and only occupies space
*/
.section .bootstrap_stack, "aw", @nobits
.align 16
  stack_bottom:
  .skip 32768
  stack_top:

.section .text
  .global _start
  .extern kmain
  .global outb
  .type _start, @function
  .type kmain, @function
  _start:
    cli

    /*Grab addresses of the stack*/
    lea edx, stack_top
    lea ecx, stack_bottom

    /*Setup stack*/
    mov esp, edx

    /*Some parameters*/
    push eax
    push ebx
    push ecx
    push edx

    /*Our main C function*/
  	call kmain
    hlt
