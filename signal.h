extern void signal(short bits) __asm__("signal");
extern void loop_8bit_3tick(short length, char* buffer) __asm__("loop_8bit_3tick");
extern void loop_out(short length, char* buffer) __asm__("loop_out");
