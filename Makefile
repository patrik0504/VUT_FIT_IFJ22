CC = gcc
CFLAGS = -Wall
LDFLAGS = -I.
TARGET = ifj_comp

OBJFILES = main.o symtable.o scanner.o stack.o expr_parser.o functions.o
DEPS = symtable.h scanner.h stack.h expr_parser.h functions.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LDFLAGS)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(TARGET) *~
