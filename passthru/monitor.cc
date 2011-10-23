#include <stdio.h>
#include <unistd.h>
#include "SerialPort.h"

int main(int argc, char **argv)
{
	SerialPort sp;
	char ch;
	int n;

	/* open the serial devices */

	if (sp.Open(argv[1]) < 0)
	{
		perror(argv[1]);
		return 1;
	}

	/* configure the ports */
	if (sp.SetParams(2400, 0, 8, 1) < 0)
		return 1;
	
	while (1)
	{
		n = sp.Read(&ch, 1);
		if (n < 0)
			break;
		write(1, &ch, 1);
	}
	
	sp.Close();
	return 0;
}

