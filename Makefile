CXXFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p
PROCESSOR=atmega328p
LDFLAGS=-mmcu=$(PROCESSOR)
CXX=avr-gcc
CC=avr-gcc

PROGNAME=main
COMPORT=/dev/ttyUSB0

$(PROGNAME).hex: $(PROGNAME)
	avr-objcopy -O ihex -R .eeprom $(PROGNAME) $(PROGNAME).hex

upload: $(PROGNAME).hex
	avrdude -v -carduino -p$(PROCESSOR) -P$(COMPORT) -b57600 -D -Uflash:w:$(PROGNAME).hex:i

main.o: main.cc

$(PROGNAME): main.o

clean:
	$(RM) *.hex
	$(RM) *.o
	$(RM) $(PROGNAME)
