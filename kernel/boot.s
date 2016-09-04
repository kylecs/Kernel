.intel_syntax noprefix

.set ALIGN, 1<<0
.set MEMINFO, 1 << 1
.set VIDEOMODE, 1<<2

.set MAGIC, 0x1BADB002
.set FLAGS, ALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)


.section .multiboot
  .align 4
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

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
    mov eax, cr0
    or al, 1
    mov cr0, eax

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
