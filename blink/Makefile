CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

MCU = atmega2560
TARGETS = blink.hex blink.lss size
OBJS = blink.o

all : $(TARGETS)

clean:
	rm -f $(TARGETS) blink.elf blink.map *.o

blink.hex : blink.elf
	$(OBJCOPY) -j .text -j .data -O ihex blink.elf  "blink.hex"

blink-eeprom.hex : blink.elf
	$(OBJCOPY) -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex blink.elf blink-eeprom.hex

size :
	$(SIZE) --format=avr --mcu=$(MCU) blink.elf

blink.lss : blink.elf
	$(OBJDUMP) -h -S blink.elf  >"blink.lss"

blink.elf : blink.o
	$(CC) -Wl,-Map,blink.map -mmcu=$(MCU) -o "blink.elf" $(OBJS)

blink.o : blink.cc
	$(CC) -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=16000000UL -c -o blink.o blink.cc
