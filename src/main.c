#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#include "vcpu.h"
#include "constants.h"

uint8_t* init_memory_from_file(char* file_x6502) {
  // Init memory
  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  // Write instructions into memory starting at 0x1000, add EXT (0xFF) opcode at end
  FILE *fp_x6502 = fopen(file_x6502, "r");
  int ins;
  int c = 0x1000;
  while ((ins = fgetc(fp_x6502)) != EOF) {
    mem[c++] = (uint8_t) ins;
  }
  mem[c++] = EXT;
  fclose(fp_x6502);

  return mem;
}

int main(int argc, char *argv[]) {
  char* file_x6502 = "a.o65"; // Hardcode xa output

  // Hardcode output file or switch to stdout
  FILE * file_out = stdout;
  file_out = fopen("out.txt", "w");

  uint8_t* mem = init_memory_from_file(file_x6502);

  run_x6502(mem, file_out);

  // Clean up
  free(mem);
  fclose(file_out);
  printf("\n");
  return 0;
}