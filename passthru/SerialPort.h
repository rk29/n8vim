#ifndef SERIALPORT_H
#define SERIALPORT_H

class OutputStream
{
	public:
		virtual int Write(const char *buffer, int size)=0;
};

class InputStream
{
	public:
		virtual int Read(char *buffer, int size)=0;
};

class SerialPort : public InputStream, public OutputStream
{
	private:
		int handle;
	public:
		SerialPort(void);
		virtual ~SerialPort(void);
		
		int Open(const char *name);
		int Close(void);
	
		int SetParams(int bps, int parity, int wordsize, int stopbits);
		int Write(const char *buffer, int size);
		int Read(char *buffer, int size);
		int SendBreak(double duration);
		int FlushInput(void);
		int FlushOutput(void);
		int GetInputBufferCount();
		int WaitForData(double timeout);
		int ReadLine(char *buffer, int size);
};

#endif
