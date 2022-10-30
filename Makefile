CC = gcc
CFLAGS = -Wall

LDFLAGS =
OBJFILES = main.o scanner.o functions.o

LDFLAGS = -I.

TARGET = ifj_comp

OBJFILES = main.o symtable.o
DEPS = symtable.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LDFLAGS)

	$(CC) $(CFLAGS) $(OBJFILES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c -o functions.o

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(TARGET) *~
