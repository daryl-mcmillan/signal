#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "signal.h"

typedef uint8_t byte;

#define SERIAL_BAUD 9600
#define SERIAL_PRESCALE ((F_CPU / ( SERIAL_BAUD * 16UL )) - 1)

void serialSetup( int baud ) {
  int prescale = (F_CPU / ( baud * 16UL )) - 1;
  UCSR0C = ( 0 << UMSEL00 ) | ( 0 << UPM00 ) | ( 0 << USBS0 )
        | ( 0b11 << UCSZ00 ) | ( 0 << UCPOL0 );
  UBRR0 = prescale;
  UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 ) | ( 1 << RXCIE0 );
  sei();
}

void serialWrite( const char c ) {
  while( !( UCSR0A & ( 1 << UDRE0 ) ) ) { }
  UDR0 = c;
}

void serialWrite( const char* str ) {
  while( *str ) {
    serialWrite( *str );
    str++;
  }
}

const char * HEX = "0123456789ABCDEF";

void serialWrite( unsigned int number ) {
  serialWrite( "0x" );
  for( int shift = 28; shift >= 0; shift -=4 ) {
    serialWrite( HEX[(number >> shift) & 0xF] );
  }
}

volatile unsigned int counter = 17;

ISR(USART_RX_vect) {
  byte val = UDR0;
  counter ++;
}

int main(void) {

  serialSetup( 19200 );
  for( ;; ) {
    serialWrite( "this is a test " );
    serialWrite( counter );
    serialWrite( "\n" );
  }

  DDRD = 0xFF;
//  for( ;; ) {
//    PORTD = 0xFF;
//    PORTD = 0x00;
//  }
  signal(0b0101010100110011);

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
