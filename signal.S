.section .text

#define PORTD 0x0B

.global loop_out
loop_out: ; ( length, buffer, stall )
  #define length r2
  #define lengthL r2
  #define lengthH r3
  #define buffer r4
  #define stall r6
  #define r_pump r18
  #define buffer_0x r19
  #define remaining r28
  #define temp r0
  #define temp16 r24
  #define temp16L r24
  #define temp16H r25

  movw length, r24
  movw buffer, r22
  movw stall, r20

  ; r24 - temp16
  ; r26 - X ( buffer pointer )
  ; r28 - remaining
  ; r30 - Z ( loop jump pointer )

  ; load last byte
  movw X, buffer
  add XL, lengthL
  adc XH, lengthH
  ld r_pump, -X

  ; preload first byte xor last byte
  movw X, buffer
  ld buffer_0x, X+
  eor buffer_0x, r_pump
  movw buffer, X

  ; adjust length to underflow
  ; at end of loop
  movw temp16, length
  sbiw temp16, 2
  ori temp16H, 0x80
  movw length, temp16

  ; setup first loop
  movw X, buffer
  movw remaining, length
  eor r_pump, buffer_0x
  rjmp loop1

reset1:
  movw X, buffer
  out PORTD, r_pump
  movw remaining, length
  eor r_pump, buffer_0x ; load and clear overflow
  rjmp loop1
  nop
  nop
  nop
loop1:
  brvs reset1
  sbiw remaining, 1
  out PORTD, r_pump
  ld r_pump, X+
  rjmp loop1
