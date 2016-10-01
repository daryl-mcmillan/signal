#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "signal.h"

typedef uint8_t byte;

#define SERIAL_BAUD 9600
#define SERIAL_PRESCALE ((F_CPU / ( SERIAL_BAUD * 16UL )) - 1)

int main(void) {

  DDRD = 0xFF;
//  for( ;; ) {
//    PORTD = 0xFF;
//    PORTD = 0x00;
//  }
  signal(0b0101010100110011);

  UCSR0C = ( 0 << UMSEL00 ) | ( 0 << UPM00 ) | ( 0 << USBS0 )
	| ( 0b11 << UCSZ00 ) | ( 0 << UCPOL0 );
  UBRR0 = SERIAL_PRESCALE;
  UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 );

  DDRB = 0b100000;
  for( ;; ) {
    // write a byte
    while( !( UCSR0A & ( 1 << UDRE0 ) ) ) { }
    UDR0 = 65;

    // read a byte
    while( !( UCSR0A & ( 1 << RXC0 ) ) ) { }
    byte val = UDR0;
    if( val == 65 ) {
      PORTB = 0b100000;
    } else if( val == 66 ) {
      PORTB = 0;
    }
  }
  return 0;

}
