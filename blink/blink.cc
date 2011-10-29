#include <avr/io.h>

void delay(void)
{
    volatile int i;
    for (i = 0; i < 500000; i++)
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
