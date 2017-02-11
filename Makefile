CC=gcc
CFLAGS=-Wall
SRC=src/vcpu.c src/main.c

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o x6502emu

clean:
	rm -f x6502emu