#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include "cpu.h"
#include "constants.h"
#include "instructions.h"
#include "memory_access.h"

#define NEGATIVE_FLAG 0x80
#define OVERFLOW_FLAG 0x40
#define BREAK_FLAG 0x10
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01

void add(vcpu* cpu, uint8_t v) {
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

void and(vcpu* cpu, uint8_t v) {
  uint16_t t = v | cpu->reg_a;
  if (t > 0xFF) {
    cpu->reg_st |= CARRY_FLAG;
  }
  cpu->reg_a = (uint8_t) t;
}

void asl(vcpu* cpu, uint8_t v) {
  uint16_t t = v << 1;
  if (t > 0xFF) {
    cpu->reg_st |= CARRY_FLAG;
  }
  cpu->reg_a = (uint8_t) t;
}

void eor(vcpu* cpu, uint8_t v) {
  cpu->reg_a = cpu->reg_a ^ v;
}

void ora(vcpu* cpu, uint8_t v) {
  cpu->reg_a = cpu->reg_a | v;
}

void inc(vcpu* cpu, uint8_t* location) {
  (*location)++;
}

void sta(vcpu* cpu, uint8_t* location) {
  (*location) = cpu->reg_a;
}

void dec(uint8_t* reg) {
  (*reg)--;
}

void cmp(vcpu* cpu, uint8_t* reg, uint8_t v) {
  if ((*reg) == v) {
    cpu->reg_st |= ZERO_FLAG;
  }
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

int run_x6502(uint8_t* mem) {
  // Struct for cpu
  vcpu* cpu = new_cpu();

  // Flag for while loop
  uint8_t RUN = 1;

  uint8_t run_instruction = 0;

  // Working vars
  uint16_t t;

  // If we had a memory write (0/1) and where
  uint8_t value_from_mem;
  uint8_t mem_access_type;
  uint8_t mem_write = 0;
  uint16_t mem_write_location = 0;
  uint8_t* mem_lookup;

  uint8_t opcode;
  uint8_t* instruction_lookup;
  uint8_t instruction;

  mem_lookup = get_memory_lookup();
  instruction_lookup = get_instruction_lookup();
  while (RUN) {

    opcode = mem[cpu->pc++];
    instruction = instruction_lookup[opcode];
    mem_access_type = mem_lookup[opcode];
    run_instruction = 1;

    switch(instruction) {
      case IN_ADC:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        add(cpu, value_from_mem);
        break;

      case IN_AND:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        and(cpu, value_from_mem);
        break;

      case IN_ASL:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        asl(cpu, value_from_mem);
        break;

      case IN_EOR:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        eor(cpu, value_from_mem);
        break;

      case IN_INC:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        inc(cpu, &mem[mem_write_location]);
        mem_write = 1;
        break;

      case IN_LDA:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cpu->reg_a = value_from_mem;
        break;

      case IN_ORA:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        ora(cpu, value_from_mem);
        break;

      case IN_STA:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        sta(cpu, &mem[mem_write_location]);
        mem_write = 1;
        break;

      default:
        // We didn't find an instruction, switch on opcode
        run_instruction = 0;
    }

    if (run_instruction == 0) {
      switch (opcode) {

        case EXT:
          RUN = 0;
          break;

        case NOP:
          cpu->pc++;
          break;

        case LDA_IM:
          cpu->reg_a = mem[cpu->pc++];
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

        case JSR_AB:
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
          inc(cpu, &(cpu->reg_x));
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
          inc(cpu, &(cpu->reg_y));
          break;

        default:
          printf("Error: unknown opcode %x, exiting\n", opcode);
          return 0;
      }
    }

    if (mem_write && mem_write_location == 0xFF00) {
      printf("%c", mem[0xFF00]);
      mem_write = 0;
      mem_write_location = 0x0000;
    }
  }

  destroy_cpu(cpu);
  printf("\n");
  return 0;
}