CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=
LDLIBS=
TARGET=RunCmd
VPATH=src
ODIR=obj
BINDIR=bin

objects=main.o command.o

$(TARGET): $(objects)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(objects)

main.o: command.h
command.o: command.h

clean:
	rm $(objects) $(BINDIR)/$(TARGET)

