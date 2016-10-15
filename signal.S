.section .text

#define PORTD 0x0B
#define SREG 0x3f

.global loop_out
loop_out: ; ( length, buffer, stall_count )
  #define length r2
  #define lengthL r2
  #define lengthH r3
  #define buffer r4
  #define stall_count r6
  #define stall_countL r6
  #define stall_countH r7
  #define r_pump r18
  #define buffer_0x r19
  #define remaining r28
  #define temp r20
  #define temp16 r24
  #define temp16L r24
  #define temp16H r25

  movw length, r24
  movw buffer, r22
  movw stall_count, r20

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

  ; set loop start
  ldi ZL, pm_lo8(loop1)
  ldi ZH, pm_hi8(loop1)
  ; apply last 3 bits
  ldi temp, 7
  and temp, stall_countL
  sub ZL, temp
  sbci ZH, 0

  ; setup first loop
  movw X, buffer
  movw remaining, length
  eor r_pump, buffer_0x
  ijmp

reset1:
  movw X, buffer
  out PORTD, r_pump
  movw remaining, length
  eor r_pump, buffer_0x ; load and clear overflow
  ijmp

  nop
  nop
  nop
  nop
  nop
  nop
  nop

stall:
  in temp, SREG
  movw temp16, stall_count
  sbiw temp16, 1
  brne stall_repeat
stall_done:
  out SREG, temp
  rjmp loop1

stall_repeat:
  nop
  nop
  nop
  sbiw temp16, 1
  breq stall_done
  rjmp stall_repeat

  nop
  nop
  nop
  nop
  nop
  nop
  nop

loop1:
  brvs reset1
  sbiw remaining, 1
  out PORTD, r_pump
  ld r_pump, X+
  ijmp
