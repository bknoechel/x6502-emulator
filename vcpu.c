#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include "opcodes.h"

#define MEMSIZE 65536

typedef struct {

  uint16_t pc;

  uint8_t reg_x;
  uint8_t reg_y;
  uint8_t reg_a;
  uint8_t reg_sp;
  uint8_t reg_st;

} vcpu;

vcpu* new_cpu() {
  vcpu* ptr = malloc(sizeof(vcpu));
  ptr->pc = 0;
  ptr->reg_x = 0;
  ptr->reg_y = 0;
  ptr->reg_a = 0;
  ptr->reg_sp = 0;
  ptr->reg_st = 0;
  return ptr;
}

int main(int argc, char *argv[]) {
  char* fn = "a.o65";

  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  FILE *f = fopen(fn, "r");
  int ins;
  int c = 0;

  while ((ins = fgetc(f)) != EOF) {
    mem[c++] = (uint8_t) ins;
  }
  mem[c++] = EXT;

  vcpu* cpu = new_cpu();

  uint8_t RUN = 1;
  uint16_t t;

  uint8_t mem_write = 0;
  uint16_t mem_write_location = 0;

  while (RUN) {

    uint8_t opcode = mem[cpu->pc++];
    switch (opcode) {
      case EXT:
        RUN = 0;
        break;

      case LDA:
        cpu->reg_a = mem[cpu->pc++];
        break;

      case STA:
        t = mem[cpu->pc] | mem[cpu->pc+1] << 8;
        cpu->pc += 2;
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      default:
        printf("Error: unknown opcode, exiting\n");
        return 0;
    }

    if (mem_write && mem_write_location == 0xFF00) {
      printf("%c", mem[0xFF00]);
      mem_write = 0;
      mem_write_location = 0x0000;
    }

  }

  return 0;
}