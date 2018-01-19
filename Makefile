CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=
LDLIBS=
TARGET=Log710Shell2
VPATH=src
ODIR=obj
BINDIR=bin

objects=main.o command.o job.o

$(TARGET): $(objects)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(objects)

main.o: command.h
command.o: command.h job.h
job.o: job.h

clean:
	rm $(objects) $(BINDIR)/$(TARGET)

run: $(TARGET)
	./$(BINDIR)/$(TARGET)
