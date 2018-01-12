CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=
LDLIBS=
TARGET=prompt
VPATH=src
ODIR=obj
BINDIR=bin

objects=main.o

$(TARGET): $(objects)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(objects)

main.o: main.h

clean:
	rm $(objects) $(BINDIR)/$(TARGET)

