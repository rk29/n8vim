#include <avr/io.h>

volatile int i;

void delay(void)
{
    for (i = 0; i < 1000; i++)
    {
    }
}

int main(void)
{
    DDRB = 0x80;

    while (1)
    {
        PORTB |= 0x80;
        delay();
        PORTB &= ~0x80;
        delay();
    }

    return 0;
}
