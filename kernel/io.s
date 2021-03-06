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

  .global outw
  outw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    mov ax, [ebp+12]
    out dx, ax
    pop ebp
    ret

  .global inw
  inw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    in ax, dx
    pop ebp
    ret

  .global outdw
  outdw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    mov eax, [ebp+12]
    out dx, eax
    pop ebp
    ret

  .global indw
  indw:
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]
    in eax, dx
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

  .global general_interrupt_handler
  .type general_interrupt_handler, @function

  .macro INTERRUPT OFFSET
    .global INTERRUPT_\OFFSET
    INTERRUPT_\OFFSET:
      cli
      pusha
      mov eax, \OFFSET
      push eax
      call general_interrupt_handler
      pop eax
      popa
      sti
      iretd
  .endm

  INTERRUPT 0
  INTERRUPT 1
  INTERRUPT 2
  INTERRUPT 3
  INTERRUPT 4
  INTERRUPT 5
  INTERRUPT 6
  INTERRUPT 7
  INTERRUPT 8
  INTERRUPT 9
  INTERRUPT 10
  INTERRUPT 11
  INTERRUPT 12
  INTERRUPT 13
  INTERRUPT 14
  INTERRUPT 15
  INTERRUPT 16
  INTERRUPT 17
  INTERRUPT 18
  INTERRUPT 19
  INTERRUPT 20
  INTERRUPT 21
  INTERRUPT 22
  INTERRUPT 23
  INTERRUPT 24
  INTERRUPT 25
  INTERRUPT 26
  INTERRUPT 27
  INTERRUPT 28
  INTERRUPT 29
  INTERRUPT 30
  INTERRUPT 31
  INTERRUPT 32
  INTERRUPT 33
  INTERRUPT 34
  INTERRUPT 35
  INTERRUPT 36
  INTERRUPT 37
  INTERRUPT 38
  INTERRUPT 39
  INTERRUPT 40
  INTERRUPT 41
  INTERRUPT 42
  INTERRUPT 43
  INTERRUPT 44
  INTERRUPT 45
  INTERRUPT 46
  INTERRUPT 47
  INTERRUPT 48
  INTERRUPT 49
  INTERRUPT 50
  INTERRUPT 51
  INTERRUPT 52
  INTERRUPT 53
  INTERRUPT 54
  INTERRUPT 55
  INTERRUPT 56
  INTERRUPT 57
  INTERRUPT 58
  INTERRUPT 59
  INTERRUPT 60
  INTERRUPT 61
  INTERRUPT 62
  INTERRUPT 63
  INTERRUPT 64
  INTERRUPT 65
  INTERRUPT 66
  INTERRUPT 67
  INTERRUPT 68
  INTERRUPT 69
  INTERRUPT 70
  INTERRUPT 71
  INTERRUPT 72
  INTERRUPT 73
  INTERRUPT 74
  INTERRUPT 75
  INTERRUPT 76
  INTERRUPT 77
  INTERRUPT 78
  INTERRUPT 79
  INTERRUPT 80
  INTERRUPT 81
  INTERRUPT 82
  INTERRUPT 83
  INTERRUPT 84
  INTERRUPT 85
  INTERRUPT 86
  INTERRUPT 87
  INTERRUPT 88
  INTERRUPT 89
  INTERRUPT 90
  INTERRUPT 91
  INTERRUPT 92
  INTERRUPT 93
  INTERRUPT 94
  INTERRUPT 95
  INTERRUPT 96
  INTERRUPT 97
  INTERRUPT 98
  INTERRUPT 99
  INTERRUPT 100
  INTERRUPT 101
  INTERRUPT 102
  INTERRUPT 103
  INTERRUPT 104
  INTERRUPT 105
  INTERRUPT 106
  INTERRUPT 107
  INTERRUPT 108
  INTERRUPT 109
  INTERRUPT 110
  INTERRUPT 111
  INTERRUPT 112
  INTERRUPT 113
  INTERRUPT 114
  INTERRUPT 115
  INTERRUPT 116
  INTERRUPT 117
  INTERRUPT 118
  INTERRUPT 119
  INTERRUPT 120
  INTERRUPT 121
  INTERRUPT 122
  INTERRUPT 123
  INTERRUPT 124
  INTERRUPT 125
  INTERRUPT 126
  INTERRUPT 127
  INTERRUPT 128
  INTERRUPT 129
  INTERRUPT 130
  INTERRUPT 131
  INTERRUPT 132
  INTERRUPT 133
  INTERRUPT 134
  INTERRUPT 135
  INTERRUPT 136
  INTERRUPT 137
  INTERRUPT 138
  INTERRUPT 139
  INTERRUPT 140
  INTERRUPT 141
  INTERRUPT 142
  INTERRUPT 143
  INTERRUPT 144
  INTERRUPT 145
  INTERRUPT 146
  INTERRUPT 147
  INTERRUPT 148
  INTERRUPT 149
  INTERRUPT 150
  INTERRUPT 151
  INTERRUPT 152
  INTERRUPT 153
  INTERRUPT 154
  INTERRUPT 155
  INTERRUPT 156
  INTERRUPT 157
  INTERRUPT 158
  INTERRUPT 159
  INTERRUPT 160
  INTERRUPT 161
  INTERRUPT 162
  INTERRUPT 163
  INTERRUPT 164
  INTERRUPT 165
  INTERRUPT 166
  INTERRUPT 167
  INTERRUPT 168
  INTERRUPT 169
  INTERRUPT 170
  INTERRUPT 171
  INTERRUPT 172
  INTERRUPT 173
  INTERRUPT 174
  INTERRUPT 175
  INTERRUPT 176
  INTERRUPT 177
  INTERRUPT 178
  INTERRUPT 179
  INTERRUPT 180
  INTERRUPT 181
  INTERRUPT 182
  INTERRUPT 183
  INTERRUPT 184
  INTERRUPT 185
  INTERRUPT 186
  INTERRUPT 187
  INTERRUPT 188
  INTERRUPT 189
  INTERRUPT 190
  INTERRUPT 191
  INTERRUPT 192
  INTERRUPT 193
  INTERRUPT 194
  INTERRUPT 195
  INTERRUPT 196
  INTERRUPT 197
  INTERRUPT 198
  INTERRUPT 199
  INTERRUPT 200
  INTERRUPT 201
  INTERRUPT 202
  INTERRUPT 203
  INTERRUPT 204
  INTERRUPT 205
  INTERRUPT 206
  INTERRUPT 207
  INTERRUPT 208
  INTERRUPT 209
  INTERRUPT 210
  INTERRUPT 211
  INTERRUPT 212
  INTERRUPT 213
  INTERRUPT 214
  INTERRUPT 215
  INTERRUPT 216
  INTERRUPT 217
  INTERRUPT 218
  INTERRUPT 219
  INTERRUPT 220
  INTERRUPT 221
  INTERRUPT 222
  INTERRUPT 223
  INTERRUPT 224
  INTERRUPT 225
  INTERRUPT 226
  INTERRUPT 227
  INTERRUPT 228
  INTERRUPT 229
  INTERRUPT 230
  INTERRUPT 231
  INTERRUPT 232
  INTERRUPT 233
  INTERRUPT 234
  INTERRUPT 235
  INTERRUPT 236
  INTERRUPT 237
  INTERRUPT 238
  INTERRUPT 239
  INTERRUPT 240
  INTERRUPT 241
  INTERRUPT 242
  INTERRUPT 243
  INTERRUPT 244
  INTERRUPT 245
  INTERRUPT 246
  INTERRUPT 247
  INTERRUPT 248
  INTERRUPT 249
  INTERRUPT 250
  INTERRUPT 251
  INTERRUPT 252
  INTERRUPT 253
  INTERRUPT 254
  INTERRUPT 255
