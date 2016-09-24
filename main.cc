#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef uint8_t byte;

#define SERIAL_BAUD 9600
#define SERIAL_PRESCALE ((F_CPU / ( SERIAL_BAUD * 16UL )) - 1)

int main(void) {
  UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 );
  UCSR0C = ( 0 << UMSEL00 ) | ( 0 << UPM00 ) | ( 0 << USBS0 )
	| ( 0b11 << UCSZ00 ) | ( 0 << UCPOL0 );
  UBRR0 = SERIAL_PRESCALE;

  DDRB = 0b100000;
  for( ;; ) {
    while( !( UCSR0A & ( 1 << UDRE0 ) ) ) { }
    UDR0 = 65;
    PORTB ^=  0b100000;
  }
  return 0;

}
