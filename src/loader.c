#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#include "vcpu.h"
#include "constants.h"

uint8_t* init_memory_from_file(FILE* fp_x6502) {
  // Init memory
  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  // Write instructions into memory starting at 0x1000, add EXT (0xFF) opcode at end
  int ins;
  int c = 0x1000;
  while ((ins = fgetc(fp_x6502)) != EOF) {
    mem[c++] = (uint8_t) ins;
  }
  mem[c++] = EXT;

  return mem;
}

void load_from_files(FILE* fp_x6502, FILE* fp_out) {
  uint8_t* mem = init_memory_from_file(fp_x6502);
  run_x6502(mem, fp_out);
  free(mem);
}

void load_from_paths(char* file_x6502, char* file_out) {
  FILE* fp_x6502 = fopen(file_x6502, "r");
  FILE* fp_out;
  if (file_out == NULL) {
    fp_out = stdout;
  } else {
    fp_out = fopen(file_out, "w");
  }

  load_from_files(fp_x6502, fp_out);

  fclose(fp_x6502);
  fclose(fp_out);
}