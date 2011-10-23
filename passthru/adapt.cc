#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <termios.h>
#include <string.h>

#include "SerialPort.h"

#define DEVICE1 "/dev/ttyS1"
#define DEVICE2 "/dev/ttyS0"
#define DEVICE3 "/dev/ttyS2"

//#define DEBUG

class Console : public OutputStream
{
	public:
		int Write(const char *buffer, int size);
};

int Console::Write(const char *buffer, int size)
{
	return write(1, buffer, size);
}

typedef struct
{
	int size;
	const char *description;
	int data;
	int offset;
} FieldInfo;

FieldInfo fields[] =
{
	{ 4, "Wind Speed (0.1kph)" },
	{ 4, "Current Wind Direction (0-255)" },
	{ 4, "5 minute Wind Speed Peak (0.1 kph)" },
	{ 4, "5 minute Wind Direction Peak (0-255) " },
	{ 4, "Wind Chill (0.1 deg F)" },
	{ 4, "Outdoor Temp (0.1deg F)" },
	{ 4, "Rain Total for today (0.01 in.)" },
	{ 4, "Barometer (0.1 mbar)" },
	{ 4, "Barometer Delta Value (0.1 mbar)" },
	{ 4, "Barometer Correction Factor(MSW)" },
	{ 4, "Indoor Temp (0.1 deg F)" },
	{ 4, "Outdoor Humidity (.1%)" },
	{ 4, "Indoor Humidity (.1%)" },
	{ 4, "Dew Point (0.1 deg F)" },
	{ 4, "Date (day of year)" },
	{ 4, "Time (minute of day)" },
	{ 4, "Today's Low Chill Value" },
	{ 4, "Today's Low Chill Time" },
	{ 4, "Yesterday's Low Chill Value" },
	{ 4, "Yesterday's Low Chill Time" },
	{ 4, "Long Term Low Chill Date" },
	{ 4, "Long Term Low Chill Value" },
	{ 4, "Long Term Low Chill Time" },
	{ 4, "Today's Low Outdoor Temp Value" },
	{ 4, "Today's Low Outdoor Temp Time" },
	{ 4, "Yesterday's Low Outdoor Temp Value" },
	{ 4, "Yesterday's Low Outdoor Temp Time" },
	{ 4, "Long Term Low Outdoor Temp Date" },
	{ 4, "Long Term Low Outdoor Temp Value" },
	{ 4, "Long Term Low Outdoor Temp Time" },
	{ 4, "Today's Low Barometer Value" },
	{ 4, "Today's Low Barometer Time" },
	{ 4, "Wind Speed (0.1kph)" },
	{ 4, "Current Wind Direction (0-255)" },
	{ 4, "Yesterday's Low Barometer Value" },
	{ 4, "Yesterday's Low Barometer Time" },
	{ 4, "Long Term Low Barometer Date" },
	{ 4, "Long Term Low Barometer Value" },
	{ 4, "Long Term Low Barometer Time" },
	{ 4, "Today's Low Indoor Temp Value" },
	{ 4, "Today's Low Indoor Temp Time" },
	{ 4, "Yesterday's Low Indoor Temp Value" },
	{ 4, "Yesterday's Low Indoor Temp Time" },
	{ 4, "Long Term Low Indoor Temp Date" },
	{ 4, "Long Term Low Indoor Temp Value" },
	{ 4, "Long Term Low Indoor Temp Time" },
	{ 4, "Today's Low Outdoor Humidity Value" },
	{ 4, "Today's Low Outdoor Humidity Time" },
	{ 4, "Yesterday's Low Outdoor Humidity Value" },
	{ 4, "Yesterday's Low Outdoor Humidity Time" },
	{ 4, "Long Term Low Outdoor Humidity Date" },
	{ 4, "Long Term Low Outdoor Humidity Value" },
	{ 4, "Long Term Low Outdoor Humidity Time" },
	{ 4, "Today's Low Indoor Humidity Value" },
	{ 4, "Today's Low Indoor Humidity Time" },
	{ 4, "Yesterday's Low Indoor Humidity Value" },
	{ 4, "Yesterday's Low Indoor Humidity Time" },
	{ 4, "Long Term Low Indoor Humidity Date" },
	{ 4, "Long Term Low Indoor Humidity Value" },
	{ 4, "Long Term Low Indoor Humidity Time" },
	{ 4, "Today's Wind Speed Value" },
	{ 4, "Today's Wind Speed Time" },
	{ 4, "Yesterday's Wind Speed Value" },
	{ 4, "Yesterday's Wind Speed Time" },
	{ 4, "Long Term Wind Speed Date" },
	{ 4, "Long Term Wind Speed Value" },
	{ 4, "Long Term Wind Speed Time" },
	{ 4, "Today's High Outdoor Temp Value" },
	{ 4, "Today's High Outdoor Temp Time" },
	{ 4, "Wind Speed (0.1kph)" },
	{ 4, "Current Wind Direction (0-255)" }, 
	{ 4, "Yesterday's High Outdoor Temp Value" }, 
	{ 4, "Yesterday's High Outdoor Temp Time" },
	{ 4, "Long Term High Outdoor Temp Date" },
	{ 4, "Long Term High Outdoor Temp Value" },
	{ 4, "Long Term High Outdoor Temp Time" },
	{ 4, "Today's High Barometer Value" },
	{ 4, "Today's High Barometer Time" },
	{ 4, "Yesterday's High Barometer Value" },
	{ 4, "Yesterday's High Barometer Time" },
	{ 4, "Long Term High Barometer Date" },
	{ 4, "Long Term High Barometer Value" },
	{ 4, "Long Term High Barometer Time" },
	{ 4, "Today's High Indoor Temp Value" },
	{ 4, "Today's High Indoor Temp Time" },
	{ 4, "Yesterday's High Indoor Temp Value" },
	{ 4, "Yesterday's High Indoor Temp Time" },
	{ 4, "Long Term High Indoor Temp Date" },
	{ 4, "Long Term High Indoor Temp Value" },
	{ 4, "Long Term High Indoor Temp Time" },
	{ 4, "Today's High Outdoor Humidity Value" },
	{ 4, "Today's High Outdoor Humidity Time" },
	{ 4, "Yesterday's High Outdoor Humidity Value" },
	{ 4, "Yesterday's High Outdoor Humidity Time" },
	{ 4, "Long Term High Outdoor Humidity Date" },
	{ 4, "Long Term High Outdoor Humidity Value" },
	{ 4, "Long Term High Outdoor Humidity Time" },
	{ 4, "Today's High Indoor Humidity Value" },
	{ 4, "Today's High Indoor Humidity Time" },
	{ 4, "Yesterday's High Indoor Humidity Value" },
	{ 4, "Yesterday's High Indoor Humidity Time" },
	{ 4, "Long Term High Indoor Humidity Date" },
	{ 4, "Long Term High Indoor Humidity Value" },
	{ 4, "Long Term High Indoor Humidity Time" },
	{ 4, "Yesterday's Rain Total (0.01 in)" }, 
	{ 4, "Long Term Rain Date" },
	{ 4, "Long Term Rain Total (0.01 in)" },
	{ 4, "Leap Year Value (0-3)" },
	{ 4, "WDCF Value (0-255)" },
	{ 2, "Yesterday's High Wind Direction (2 bytes)" },
	{ 2, "Today's High Wind Direction (2 bytes)" },
	{ 2, "Spare (2 bytes)" },
	{ 2, "Long Term High Wind Direction (2 bytes)" },
	{ 4, "1 Minute Wind Speed Average (0.1kph)" },
	{ 0, 0 }
};

static int convert(const char *buffer, int size, int *value)
{
	int i;
	int digit;
	
	*value = 0;
	for (i=0; i<size; i++)
	{
		if (*buffer == 0)
			return -1;
		if (*buffer >= '0' && *buffer <= '9')
			digit = *buffer - '0';
		else if (*buffer >= 'A' && *buffer <= 'F')
			digit = *buffer - 'A' + 10;
		else if (*buffer >= 'a' && *buffer <= 'f')
			digit = *buffer - 'a' + 10;
		*value = (*value * 16) + digit;
	}
	return 0;
}

int parse_record(const char *buffer)
{
	FieldInfo *field;
	int offset=0;
	
	if (strncmp(buffer, "&CR&", 4) != 0)
		return -1;
	buffer += 4;

	for (field=fields; field->size; field++)
	{
		field->offset = offset;
		if (strlen(buffer) < field->size)
			return -1;
		if (convert(buffer, field->size, &field->data) < 0)
			return -1;
		buffer += field->size;
		offset += field->size;
	}
	return 0;
}

void show_field(const char *buffer, int fieldnum, OutputStream *out)
{
	out->Write(buffer + fields[fieldnum].offset, fields[fieldnum].size);
}

int main(int argc, char **argv)
{
	SerialPort sp1;
	SerialPort sp2;
#ifdef DEBUG
	Console sp3;
#else
	SerialPort sp3;
#endif	
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
#ifndef DEBUG
	if (sp3.Open(DEVICE3) < 0)
	{
		perror(DEVICE3);
		return 1;
	}
#endif

	/* configure the ports */
	if (sp1.SetParams(2400, 0, 8, 1) < 0)
		return 1;
	if (sp2.SetParams(2400, 0, 8, 1) < 0)
		return 1;
#ifndef DEBUG
	if (sp3.SetParams(2400, 0, 8, 1) < 0)
		return 1;
#endif

	/* read a line of data from the port */	
	while (1)
	{
		n = sp1.ReadLine(buffer, sizeof(buffer));
		if (n < 0)
			break;
		buffer[n] = 0;
	
		if (strncmp(buffer, "&CR&", 4) == 0)
		{
			sp2.Write(buffer, n);
			sp2.Write("\r\n", 2);
		}
		if (parse_record(buffer) >= 0)
		{
			sp3.Write("$ULTW", 5);
			sp3.Write(buffer + 3*4, 4);
			sp3.Write(buffer + 4*4, 4);
			sp3.Write(buffer + 6*4, 4);
			sp3.Write(buffer + 108*4, 4);
			sp3.Write(buffer + 8*4, 4);
			sp3.Write(buffer + 9*4, 4);
			sp3.Write(buffer + 10*4, 4);
			sp3.Write(buffer + 11*4, 4);
			sp3.Write(buffer + 13*4, 4);
			sp3.Write(buffer + 16*4, 4);
			sp3.Write(buffer + 17*4, 4);
			sp3.Write(buffer + 7*4, 4);
			sp3.Write(buffer + 113*4, 4);
			sp3.Write("\r\n", 2);
		}
	}

	/* close the serial device */
	sp1.Close();
	sp2.Close();
//	sp3.Close();
	return 0;
}

