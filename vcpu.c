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

void add(vcpu* cpu, uint8_t* mem, uint8_t v) {
  uint16_t t;
  if (cpu->reg_st & DECIMAL_FLAG) {

  } else {
    t = v + cpu->reg_a;
    if (t > 0xFF) {
      cpu->reg_st |= CARRY_FLAG;
    }
    cpu->reg_a = (uint8_t) t;
  }
}

void inc(uint8_t* reg) {
  (*reg)++;
}

void dec(uint8_t* reg) {
  (*reg)--;
}

void cmp(vcpu* cpu, uint8_t* reg, uint8_t v) {
  if ((*reg) == v) {
    cpu->reg_st |= ZERO_FLAG;
  }
}

int main(int argc, char *argv[]) {
  char* fn = "a.o65"; // Hardcode xa output

  // Memory (not part of cpu struct)
  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  // Write assembly into memory starting at 0x0000, add EXT (0xFF) opcode at end
  FILE *f = fopen(fn, "r");
  int ins;
  int c = 0;
  while ((ins = fgetc(f)) != EOF) {
    mem[c++] = (uint8_t) ins;
  }
  mem[c++] = EXT;

  // Struct for cpu
  vcpu* cpu = new_cpu();

  // Flag for while loop
  uint8_t RUN = 1;

  // Working vars
  uint16_t t;

  // If we had a memory write (0/1) and where
  uint8_t mem_write = 0;
  uint16_t mem_write_location = 0;

  while (RUN) {

    uint8_t opcode = mem[cpu->pc++];
    switch (opcode) {

      case EXT:
        RUN = 0;
        break;

      // Add
      case ADC_IM:
        add(cpu, mem, mem[cpu->pc++]);
        break;

      case LDA_IM:
        cpu->reg_a = mem[cpu->pc++];
        break;

      // Store
      case STA:
        t = mem[cpu->pc] | mem[cpu->pc+1] << 8;
        cpu->pc += 2;
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      // Branch
      case BPL:
        break;

      case BMI:
        break;

      case BVC:
        break;

      case BVS:
        break;

      case BCC:
        break;

      case BCS:
        break;

      case BNE:
        if (!(cpu->reg_st & ZERO_FLAG)) {
          cpu->pc += (int8_t) mem[cpu->pc];
        }
        cpu->pc++;
        break;

      case BEQ:
        break;

      // Compare
      case CMP_IM:
        cmp(cpu, &(cpu->reg_a), mem[cpu->pc++]);
        break;

      case CMP_AB:
        t = mem[cpu->pc] | mem[cpu->pc+1] << 8;
        cpu->pc += 2;
        cmp(cpu, &(cpu->reg_a), mem[t]);
        break;

      // Increment memory
      case INC_AB:
        t = mem[cpu->pc] | mem[cpu->pc+1] << 8;
        cpu->pc += 2;
        mem[t]++;
        mem_write = 1;
        mem_write_location = t;
        break;


      // Transfer, increment, decrement x/y
      case TAX:
        cpu->reg_x = cpu->reg_a;
        break;

      case TXA:
        cpu->reg_a = cpu->reg_x;
        break;

      case DEX:
        dec(&(cpu->reg_x));
        break;

      case INX:
        inc(&(cpu->reg_x));
        break;

      case TAY:
        cpu->reg_y = cpu->reg_a;
        break;

      case TYA:
        cpu->reg_a = cpu->reg_y;
        break;

      case DEY:
        dec(&(cpu->reg_y));
        break;

      case INY:
        inc(&(cpu->reg_y));
        break;

      default:
        printf("Error: unknown opcode %x, exiting\n", opcode);
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