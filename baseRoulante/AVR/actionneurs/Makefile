CC=avr-gcc
CFLAGS=-Wall -O2 
MMCU_GCC=atmega328p
MMCU_DUDE=m328p
FREQ=16000000

all: main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

asereje.hex: asereje.out
	avr-objcopy -O ihex -R .eeprom asereje.out asereje.hex

asereje.out: asereje.o actionneurs.o ax12.o twi_slave.o adc.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU_GCC) -DF_CPU=$(FREQ) -o asereje.out asereje.o actionneurs.o ax12.o twi_slave.o adc.o 

main.out: main.o actionneurs.o ax12.o twi_slave.o adc.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU_GCC) -DF_CPU=$(FREQ) main.o adc.o actionneurs.o twi_slave.o ax12.o -o main.out

%.o: %.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU_GCC) -DF_CPU=$(FREQ) -c $<

clean:
	rm -rf *.o *.hex *.out *~

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p $(MMCU_DUDE) -b 57600 -D -U flash:w:main.hex

upload2: all
	sudo avrdude -c usbasp -p $(MMCU_DUDE) -U flash:w:main.hex

upload3: all
	sudo avrdude -c usbtiny -p $(MMCU_DUDE) -U flash:w:main.hex
