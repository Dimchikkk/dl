CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared
TARGET = libhello.so
HOST = host

.PHONY: all clean

all: $(HOST)

$(HOST): host.o $(TARGET)
	$(CC) -o $@ host.o -ldl

host.o: host.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): hello1.o
	$(CC) $(LDFLAGS) -o $@ $<

hello1.o: hello1.c
	$(CC) $(CFLAGS) -c $< -o $@

hello2.o: hello2.c
	$(CC) $(CFLAGS) -c $< -o $@

hello1: hello1.o
	$(CC) $(LDFLAGS) -o $(TARGET) hello1.o

hello2: hello2.o
	$(CC) $(LDFLAGS) -o $(TARGET) hello2.o

clean:
	rm -f *.o $(TARGET) $(HOST)
