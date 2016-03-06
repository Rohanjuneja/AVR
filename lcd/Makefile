F_CPU = 16000000UL	#arduino clock rate
CC = avr-gcc	# compiler
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) # -Wall turn on compiler warnings, -Os makes the compiles optimize the code
MCU = atmega328p
PORT = /dev/ttyACM0
BAUD_RATE = 115200	# Data rate (in bits per second) between microcontroller and computer
AVRDUDE = avrdude -F -V
RM = rm -rf
FILE = lcd

all: upload

compile: $(FILE).hex

$(FILE).hex: $(FILE).elf
	avr-objcopy -O ihex -R .eeprom $^ $@

$(FILE).elf: $(FILE).c
	$(CC) $(CFLAGS) -o $@ $^

upload: $(FILE).hex
	sudo $(AVRDUDE) -c arduino -P $(PORT) -p $(MCU) -b $(BAUD_RATE) -U flash:w:$(FILE).hex

clean:
	$(RM) *.elf *.hex *.o

#350 bytes of flash written