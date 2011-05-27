CC=avr-gcc
CFLAGS=-Wall -O2
MMCU=atmega328p
FREQ=20000000

all: main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

main.out: main.o serial.o ultrasons.o twi_slave.o fin_course.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o serial.o ultrasons.o twi_slave.o fin_course.o -o main.out

main.o: main.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.c -c

ultrasons.o: ultrasons.c ultrasons.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ultrasons.c -c

serial.o: serial.c serial.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) serial.c -c

twi_slave.o: twi_slave.c twi_slave.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) twi_slave.c -c

fin_course.o: fin_course.c fin_course.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) fin_course.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:main.hex

upload2: all
	sudo avrdude -c usbasp -p m328p -U flash:w:main.hex

upload3: all
	sudo avrdude -c usbtiny -p m328p -U flash:w:main.hex

 