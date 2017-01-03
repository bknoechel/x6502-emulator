CC=gcc
CFLAGS=-Wall

all: vcpu.c
	$(CC) $(CFLAGS) vcpu.c -o out

clean:
	rm -f out