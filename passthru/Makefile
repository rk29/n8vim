CFLAGS = -Wall -Werror

PASSTHRU_OBJS = \
	SerialPort.o \
	passthru.o

MONITOR_OBJS = \
	SerialPort.o \
	monitor.o

ADAPT_OBJS = \
	SerialPort.o \
	adapt.o

all : passthru monitor adapt

passthru : $(PASSTHRU_OBJS)
	$(CXX) -o passthru $(PASSTHRU_OBJS)

monitor : $(MONITOR_OBJS)
	$(CXX) -o monitor $(MONITOR_OBJS)

adapt : $(ADAPT_OBJS)
	$(CXX) -o adapt $(ADAPT_OBJS)

clean :
	rm -f *.o

dep :
	gcc -M *.cc > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

