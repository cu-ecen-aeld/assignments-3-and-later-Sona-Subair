SRC := aesdsocket.c
TARGET = aesdsocket
#OBJS := $(SRC:.c=.o)

CC=gcc
CROSS_COMPILE= 
LDFLAGS= -pthread  -lpthread

all: aesdsocket

$(TARGET) : $(OBJS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map
