CC = gcc
CFLAGS = -Wall -g
TARGET = testlib
OBJS = main.o list.o hash.o bitmap.o hex_dump.o debug.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)