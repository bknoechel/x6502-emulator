CC=gcc
CFLAGS=-Wall

all: vcpu.c
	$(CC) $(CFLAGS) vcpu.c main.c -o out

clean:
	rm -f out