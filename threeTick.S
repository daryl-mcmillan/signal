.section .text

#define PORTD 0x0B

.global loop_8bit_3tick
loop_8bit_3tick: ; ( length, buffer ) 
  #define buffer r22
  #define length r24
  #define length_low r24
  #define length_high r25

  #define r_pump r18
  #define buffer_0 r19
  #define buffer_1 r20
  #define buffer_2 r21

  #define MAX_BUFFER 60

  ; pop 3 bytes off the buffer
  movw X, buffer
  ld buffer_0, X+
  ld buffer_1, X+
  ld buffer_2, X+
  movw buffer, X
  sbiw length, 3

  ; Z = end_of_loop - 2 * length
  ldi ZL, pm_lo8(end_of_loop)
  ldi ZH, pm_hi8(end_of_loop)
  sub ZL, length_low
  sbc ZH, length_high
  sub ZL, length_low
  sbc ZH, length_high

loop:
  out PORTD, buffer_0
  movw X, buffer
  nop
  out PORTD, buffer_1
  ld r_pump, X+
  out PORTD, buffer_2
  ijmp
start_of_loop:
  .rept MAX_BUFFER
  out PORTD, r_pump
  ld r_pump, X+
  .endr
  out PORTD, r_pump
  rjmp loop
end_of_loop:
  nop
