#include <avr/io.h>

#include "Serial.h"

Serial serial1(0, 2400);
Serial serial2(1, 2400);

int main(void)
{
    while (1)
    {
      uint8_t b;
      if (serial1.get(b))
      {
        serial2.put(b);
      }
    }

    return 0;
}
