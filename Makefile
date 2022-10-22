CC = gcc
CFLAGS = -Wall
LDFLAGS = -I.
TARGET = ifj_comp

OBJFILES = main.o symtable.o
DEPS = symtable.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LDFLAGS)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(TARGET) *~
