APP = passthru
MCU = atmega2560
TARGETS = $(APP).hex $(APP).lss size
OBJS = passthru.o Serial.o

CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
CXXFLAGS = -Wall -Werror -O2 -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=16000000UL

all : $(TARGETS)

clean:
	rm -f $(TARGETS) $(APP).elf $(APP).map *.o *~

$(APP).hex : $(APP).elf
	$(OBJCOPY) -j .text -j .data -O ihex $(APP).elf $(APP).hex

$(APP)-eeprom.hex : $(APP).elf
	$(OBJCOPY) -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $(APP).elf $(APP)-eeprom.hex

size :
	$(SIZE) --format=avr --mcu=$(MCU) $(APP).elf

$(APP).lss : $(APP).elf
	$(OBJDUMP) -h -S $(APP).elf  > $(APP).lss

$(APP).elf : $(OBJS)
	$(CC) -Wl,-Map,$(APP).map -mmcu=$(MCU) -o $(APP).elf $(OBJS)

