.intel_syntax noprefix
.section .text
  .global outw
  .global inw
  .type outw, @function
  .type inw @function

  /*
  Writes 2 bytes to given port
  Takes 2 bytes address, then 2 bytes of data
  */
  outw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    mov ax, [ebp+12]
    out dx, ax
    pop ebp
    ret

  /*
  Reads 2 bytes from the given address
  */
  inw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    in ax, dx
    pop ebp
    ret
