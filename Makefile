CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = main.o scanner.o
TARGET = ifj_comp

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o