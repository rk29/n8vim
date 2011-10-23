#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "SerialPort.h"

#define DEVICE1 "/dev/ttyS1"
#define DEVICE2 "/dev/ttyS0"

int main(int argc, char **argv)
{
	SerialPort sp1;
	SerialPort sp2;
	char buffer[1024];
	int n;

	/* open the serial devices */

	if (sp1.Open(DEVICE1) < 0)
	{
		perror(DEVICE1);
		return 1;
	}
	if (sp2.Open(DEVICE2) < 0)
	{
		perror(DEVICE2);
		return 1;
	}

	/* configure the ports */
	if (sp1.SetParams(2400, 0, 8, 1) < 0)
		return 1;
	if (sp2.SetParams(2400, 0, 8, 1) < 0)
		return 1;

	/* read a line of data from the port */	
	while (1)
	{
		n = sp1.ReadLine(buffer, sizeof(buffer));
		if (n < 0)
			return 1;
		buffer[n] = 0;
	
		if (strncmp(buffer, "&CR&", 4) == 0)
		{
			sp2.Write(buffer, n);
			sp2.Write("\r\n", 2);
		}
	}

	/* close the serial device */
	sp1.Close();
	sp2.Close();
	return 0;
}

