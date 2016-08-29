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
  stack_bottom:
  .skip 8192
  stack_top:

.section .text
  .global _start
  .extern kmain
  .global testfunc
  .global inb
  .global outb
  .type _start, @function
  .type kmain, @function
  .type testfunc, @function
  .type inb, @function
  .type outb, @function

  _start:
    cli
    lea edx, stack_top
    lea ecx, stack_bottom
    mov esp, edx
    push eax
    push ebx
    push ecx
    push edx
  	call kmain
    hlt

  testfunc:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov edx, [ebp+12]
    add eax, edx
    pop ebp
    ret



.section HANG
  jmp HANG
