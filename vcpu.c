#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include "opcodes.h"

#define MEMSIZE 65536

#define NEGATIVE_FLAG 0x80
#define OVERFLOW_FLAG 0x40
#define BREAK_FLAG 0x10
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01

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
  uint8_t n;
  uint16_t t;

  uint8_t mem_write = 0;
  uint16_t mem_write_location = 0;

  while (RUN) {

    uint8_t opcode = mem[cpu->pc++];
    switch (opcode) {

      case EXT:
        RUN = 0;
        break;

      case ADC_IM:
        n = mem[cpu->pc++];
        if (cpu->reg_st & DECIMAL_FLAG) {

        } else {
          t = n + cpu->reg_a;
          if (t > 0xFF) {
            cpu->reg_st |= CARRY_FLAG;
          }
          cpu->reg_a = (uint8_t) t;
        }
        break;

      case LDA_IM:
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