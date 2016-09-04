.intel_syntax noprefix
.section .text
  .global outb
  .type outb, @function

  .global inb
  .type inb, @function

  .global load_gdt
  .type load_gdt, @function

  .global load_idt
  .type load_idt, @function

  .global enable_interrupts
  .type enable_interrupts, @function

  .global disable_interrupts
  .type disable_interrupts, @function

  .global handle_interrupt
  .type handle_interrupt, @function

  .extern c_handle_interrupt
  .type c_handle_interrupt, @function

  .global flush_gdt
  .type flush_gdt, @function

  .global gpf_handler
  .type gpf_handler, @function

  .extern c_gpf_handler
  .type c_gpf_handler, @function

  /*
  Writes 1 bytesto given port
  Takes 2 bytes address, then 1 bytes of data
  */
  outb:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    mov al, [ebp+12]
    out dx, al
    pop ebp
    ret

  /*
  Reads 2 bytes from the given address
  */
  inb:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    in al, dx
    pop ebp
    ret
  /*
  calls lgdt
  */
  load_gdt:
    lgdt [esp+4]
    jmp 0x08:flush_gdt
    ret

  flush_gdt:
    mov ax, 0x10
    mov ds, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

  load_idt:
    lidt [esp+4]
    ret

  enable_interrupts:
    sti
    ret

  disable_interrupts:
    cli
    ret

  handle_interrupt:
    call c_handle_interrupt
    iretd

  gpf_handler:
    cli
    call c_gpf_handler
    sti
    iret
