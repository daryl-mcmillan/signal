#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "signal.h"
#include "serial.h"

volatile unsigned int counter = 0;

void charReceived( unsigned char c ) {
  counter ++;
}

int length = 0;
char buffer[1500];
void add( char c ) {
  buffer[length] = c;
  length ++;
}

int main(void) {

  DDRD = 0xFF;

  /*
  for( int i=0; i<63; i++ ) {
    add( i << 2 );
  }
  for( int i=62; i>=1; i-- ) {
    add( i << 2 );
  }
  */
  add( 0 );
  add( 255 );
  add( 0 );
  add( 0 );
  add( 255 );
  add( 255 );

  loop_out( length, buffer, 20000 );
  //loop_8bit_3tick( length, buffer );

  serialSetupHandler( 19200, &charReceived );
  //serialSetup( 19200 );
  for( ;; ) {
    serialWrite( "this is a test " );
    serialWrite( counter );
    serialWrite( "\n" );
  }

  DDRD = 0xFF;
  return 0;

}
