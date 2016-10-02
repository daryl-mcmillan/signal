#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "signal.h"

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
  for( int shift = 28; shift >= 0; shift -= 4 ) {
    serialWrite( HEX[(number >> shift) & 0xF] );
  }
}

volatile unsigned int counter = 0;

ISR(USART_RX_vect) {
  char val = UDR0;
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
  signal(0b0101010100110011);
  return 0;

}
