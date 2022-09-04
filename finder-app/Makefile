SRC := writer.c
TARGET = writer
OBJS := $(SRC:.c=.o)

CC=gcc
CR_C = 

all:$(TARGET)

$(TARGET) : $(OBJS)
	$(CR_C)$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map