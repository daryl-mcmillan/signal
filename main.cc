#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "signal.h"
#include "serial.h"

volatile unsigned int counter = 0;

void charReceived( unsigned char c ) {
  counter ++;
}

int main(void) {

  DDRD = 0xFF;
  char buffer[1000];
  int length = 0;
//  for( int i=0; i<63; i++ ) {
//    buffer[length] = i << 2;
//    length ++;
//  }
//  for( int i=62; i>=1; i-- ) {
//    buffer[length] = i << 2;
//    length ++;
//  }
  buffer[0] = 0;
  buffer[1] = 255;
  buffer[2] = 0;
  buffer[3] = 255;
  buffer[4] = 0;
  buffer[5] = 0;
  buffer[6] = 255;
  buffer[7] = 255;
  buffer[8] = 0;
  buffer[9] = 0;
  buffer[10] = 0;
  length = 11;

  //loop_out( length, buffer );
  //loop_8bit_3tick( length, buffer );

  serialSetupHandler( 19200, &charReceived );
  //serialSetup( 19200 );
  for( ;; ) {
    serialWrite( "this is a test " );
    serialWrite( counter );
    serialWrite( "\n" );
  }

  DDRD = 0xFF;
  signal(0b0101010100110011);
  return 0;

}
