F_CPU = 16000000UL
CC = avr-gcc
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
MCU = atmega328p
PORT = /dev/ttyACM0
BAUD_RATE = 115200
AVRDUDE = avrdude -F -V
RM = rm -rf
FILE = blink

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

#166 bytes of flash written