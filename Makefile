CC = gcc
CFLAGS = -Wall
LDFLAGS = -I.
TARGET = ifj_comp

OBJFILES = main.o symtable.o scanner.o functions.o parser.o
DEPS = symtable.h scanner.h functions.h parser.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LDFLAGS)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(TARGET) *~
