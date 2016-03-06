SRC=adc
CXX=avr-gcc
MCU=-mmcu=atmega328p #-- CPU
CPU_SPEED=-DF_CPU=16000000UL #-- clock speed of the board (mentioned on the quartz)
CFLAGS=$(MCU) $(CPU_SPEED) -Os -w -Wl,--gc-sections -ffunction-sections -fdata-sections
PORT=/dev/ttyACM0


all:
	$(CXX) $(CFLAGS) $(SRC).c -o $(SRC).elf
	avr-objcopy -O ihex $(SRC).elf $(SRC).hex
	avrdude -V -F -p m328p -c arduino -b 115200 -Uflash:w:$(SRC).hex -P$(PORT)

clean:
	@echo -n Cleaning ...
	$(shell rm $(SRC).elf 2> /dev/null)
	$(shell rm $(SRC).hex 2> /dev/null)