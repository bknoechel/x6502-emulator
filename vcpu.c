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
  ptr->pc = 0x1000;
  ptr->reg_x = 0;
  ptr->reg_y = 0;
  ptr->reg_a = 0;
  ptr->reg_sp = 0xFF;
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

// Return memory location by reading two consecutive bytes in memory to get
// uint16_t address
uint16_t abs_mem(vcpu* cpu, uint8_t* mem) {
  uint16_t m = mem[cpu->pc] | mem[cpu->pc+1] << 8;
  cpu->pc += 2;
  return m;
}

uint16_t absx_mem(vcpu* cpu, uint8_t* mem) {
  uint16_t m = abs_mem(cpu, mem) + cpu->reg_x;
  return m;
}

uint16_t absy_mem(vcpu* cpu, uint8_t* mem) {
  uint16_t m = abs_mem(cpu, mem) + cpu->reg_y;
  return m;
}

uint16_t zero_mem(vcpu* cpu, uint8_t* mem) {
  return (uint16_t)mem[cpu->pc++];
}

uint16_t zerox_mem(vcpu* cpu, uint8_t* mem) {
  uint16_t m = zero_mem(cpu, mem) + cpu->reg_x;
  return m & 0x00FF;
}

// Stack lives in memory between 0x0100 and 0x01ff, reg_sp initially at 0xff

void stack_push(vcpu* cpu, uint8_t* mem, uint8_t v) {
  cpu->reg_sp--;
  uint16_t m = cpu->reg_sp | 0x0100;
  mem[m] = v;
}

uint8_t stack_pull(vcpu* cpu, uint8_t* mem) {
  uint16_t m = cpu->reg_sp | 0x0100;
  uint8_t v = mem[m];
  cpu->reg_sp++;
  return v;
}

int main(int argc, char *argv[]) {
  char* fn = "a.o65"; // Hardcode xa output

  // Memory (not part of cpu struct)
  uint8_t* mem = (uint8_t*) malloc(sizeof(uint8_t)*MEMSIZE);

  // Write assembly into memory starting at 0x0000, add EXT (0xFF) opcode at end
  FILE *f = fopen(fn, "r");
  int ins;
  int c = 0x1000;
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

      case NOP:
        cpu->pc++;
        break;

      // Add
      case ADC_IM:
        add(cpu, mem, mem[cpu->pc++]);
        break;

      case ADC_ZE:
        t = zero_mem(cpu, mem);
        add(cpu, mem, mem[t]);
        break;

      case ADC_ZX:
        t = zerox_mem(cpu, mem);
        add(cpu, mem, mem[t]);
        break;

      case ADC_AB:
        t = abs_mem(cpu, mem);
        add(cpu, mem, mem[t]);
        break;

      case ADC_AX:
        t = absx_mem(cpu, mem);
        add(cpu, mem, mem[t]);
        break;

      case ADC_AY:
        t = absy_mem(cpu, mem);
        add(cpu, mem, mem[t]);
        break;

      case LDA_IM:
        cpu->reg_a = mem[cpu->pc++];
        break;

      // Store
      case STA_ZE:
        t = zero_mem(cpu, mem);
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      case STA_ZX:
        t = zerox_mem(cpu, mem);
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      case STA_AB:
        t = abs_mem(cpu, mem);
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      case STA_AX:
        t = absx_mem(cpu, mem);
        mem[t] = cpu->reg_a;
        mem_write = 1;
        mem_write_location = t;
        break;

      case STA_AY:
        t = absy_mem(cpu, mem);
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

      // Jump

      case JMP_AB:
        t = abs_mem(cpu, mem);
        cpu->pc = t;
        break;

      case JSR:
        t = cpu->pc + 1; // Next op - 1
        stack_push(cpu, mem, (uint8_t)((t & 0xff00) >> 8));
        stack_push(cpu, mem, (uint8_t)(t & 0x00ff));
        t = abs_mem(cpu, mem);
        cpu->pc = t;
        break;

      case RTS:
        t = (uint16_t)stack_pull(cpu, mem);
        t = t | (uint16_t)(stack_pull(cpu, mem) << 8);
        cpu-> pc = t + 1;
        break;

      // Stack
      case TXS:
        break;

      case PHA:
        stack_push(cpu, mem, cpu->reg_a);
        break;

      case PLA:
        cpu->reg_a = stack_pull(cpu, mem);
        break;

      // Compare
      case CMP_IM:
        cmp(cpu, &(cpu->reg_a), mem[cpu->pc++]);
        break;

      case CMP_AB:
        t = abs_mem(cpu, mem);
        cmp(cpu, &(cpu->reg_a), mem[t]);
        break;

      // Increment memory
      case INC_AB:
        t = abs_mem(cpu, mem);
        mem[t]++;
        mem_write = 1;
        mem_write_location = t;
        break;

      // Load x,y
      case LDX_IM:
        cpu->reg_x = mem[cpu->pc++];
        break;

      case LDY_IM:
        cpu->reg_y = mem[cpu->pc++];
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

  printf("\n");
  return 0;
}