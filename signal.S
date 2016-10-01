.section .text
.global signal
signal:
  movw r18, r24
  .rept 8
  out 0x0B, r19
  lsl r19
  .endr
  .rept 8
  out 0x0B, r18
  lsl r18
  .endr
  rjmp signal
