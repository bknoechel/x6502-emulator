#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#include "vcpu.h"
#include "constants.h"

int main(int argc, char *argv[]) {

  char* fn = "a.o65"; // Hardcode xa output

  // Memory
  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  // Write instructions into memory starting at 0x1000, add EXT (0xFF) opcode at end
  FILE *f = fopen(fn, "r");
  int ins;
  int c = 0x1000;
  while ((ins = fgetc(f)) != EOF) {
    mem[c++] = (uint8_t) ins;
  }
  mem[c++] = EXT;

  run_x6502(mem);

  return 0;
}