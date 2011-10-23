#include "SerialPort.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

SerialPort::SerialPort(void)
{
	handle = -1;
}

SerialPort::~SerialPort(void)
{
	Close();
}

int SerialPort::Open(const char *name)
{
	handle = open(name, O_CREAT | O_RDWR, 0666);
	if (handle < 0)
		return -1;
	return 0;
}

int SerialPort::Close(void)
{
	close(handle);
	handle = -1;
	return 0;
}

int SerialPort::SetParams(int bps, int parity, int wordsize, int stopbits)
{
	struct termios t;

	/* set the tty to RAW mode */
	tcgetattr(handle, &t);
	cfmakeraw(&t);
		
	/* FIXME use all settings */
	switch (bps)
	{
		case 38400:
			cfsetispeed(&t, B38400);
			cfsetospeed(&t, B38400);
			break;
		case 19200:
			cfsetispeed(&t, B19200);
			cfsetospeed(&t, B19200);
			break;
		case 9600:
			cfsetispeed(&t, B9600);
			cfsetospeed(&t, B9600);
			break;
		case 4800:
			cfsetispeed(&t, B4800);
			cfsetospeed(&t, B4800);
			break;
		case 2400:
			cfsetispeed(&t, B2400);
			cfsetospeed(&t, B2400);
			break;
		case 1200:
			cfsetispeed(&t, B1200);
			cfsetospeed(&t, B1200);
			break;
		case 300:
			cfsetispeed(&t, B300);
			cfsetospeed(&t, B300);
			break;
	}
	tcsetattr(handle, TCSANOW, &t);

	return 0;
}

int SerialPort::Write(const char *buffer, int size)
{
	return write(handle, buffer, size);
}

int SerialPort::Read(char *buffer, int size)
{
	return read(handle, buffer, size);
}

int SerialPort::SendBreak(double duration)
{
	return tcsendbreak(handle, (int) (duration*1000));
}

int SerialPort::FlushInput()
{
	return tcflush(handle, TCIFLUSH);
}

int SerialPort::FlushOutput()
{
	return tcdrain(handle);
}

int SerialPort::GetInputBufferCount()
{
	int arg=0;

	if (ioctl(handle, TIOCINQ, &arg) < 0)
		return -1;
	return (int) arg;
}

int SerialPort::WaitForData(double timeout)
{
	fd_set fds;
	struct timeval tv;

	FD_ZERO(&fds);
	FD_SET(handle, &fds);
	tv.tv_sec = (long) timeout;
	tv.tv_usec = (long) ((timeout - tv.tv_sec) * 1e6);
	select(handle+1, &fds, NULL, NULL, &tv);
	return GetInputBufferCount();
}

int SerialPort::ReadLine(char *buffer, int size)
{
	int rc;
	char ch;
	int n = 0;

	while (n < size)
	{
		if (WaitForData(1.0) == 0)
			return 0;

		rc = Read(&ch, 1);
		if (rc < 0)
		{
			perror("read_line");
			return -1;
		}
		if (rc == 0)
		{
			printf("timeout\n");
			return 0;
		}
		if (ch != '\r' && ch != '\n')
		{
			buffer[n++] = ch;
		}

		if (ch == '\r')
		{
			return n;
		}
	}
	return n;
}

