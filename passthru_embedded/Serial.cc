#include <inttypes.h>
#include <avr/io.h>

#include "Serial.h"

// bits in UCSRB

#define TXEN (1<<3)
#define RXEN (1<<4)
#define RXCIE (1<<7)

// bits in UCSRA

#define UDRE (1<<5)
#define RXC (1<<7)

struct SerialRegisters
{
  volatile uint8_t &UBRRH;
  volatile uint8_t &UBRRL;
  volatile uint8_t &UCSRA;
  volatile uint8_t &UCSRB;
  volatile uint8_t &UDR;
};

static SerialRegisters registers[] =
{
  { UBRR0H, UBRR0L, UCSR0A, UCSR0B, UDR0 },
  { UBRR1H, UBRR1L, UCSR1A, UCSR1B, UDR1 },
  { UBRR2H, UBRR2L, UCSR2A, UCSR2B, UDR2 },
  { UBRR3H, UBRR3L, UCSR3A, UCSR3B, UDR3 },
};

Serial::Serial(int portNum, long bps) : regs(registers[portNum])
{
  regs.UBRRH = ((F_CPU / 16 + bps / 2) / bps - 1) >> 8;
  regs.UBRRL = ((F_CPU / 16 + bps / 2) / bps - 1);
  regs.UCSRB |= (RXEN|TXEN); // TODO add RXCIE and interrupts
}

uint8_t Serial::get(uint8_t &b)
{
  if (regs.UCSRA & RXC)
  {
    b = regs.UDR;
    return 1;
  }
  return 0;
}

uint8_t Serial::put(uint8_t b)
{
  while (!(regs.UCSRA & UDRE))
  {
  }
  regs.UDR = b;
  return 1;
}