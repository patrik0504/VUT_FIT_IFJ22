CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = main.o
TARGET = ifj_comp

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

main.o : scanner.h