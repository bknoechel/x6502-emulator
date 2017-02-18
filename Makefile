CC=gcc
VCPU=src/vcpu.c src/main.c src/loader.c src/loader.h
TEST=src/vcpu.c test/test.c src/loader.c src/loader.h
TEST_DIR=./test
CFLAGS=-Wall
DEPS=src/loader.h

all: $(VCPU)
	$(CC) $(CFLAGS) $(VCPU) -o x6502emu

test: $(TEST)
	$(CC) $(CFLAGS) $(TEST) -o x6502emu-test

clean:
	rm -f x6502emu x6502emu-test test/*.o65 test/results.txt