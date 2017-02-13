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


/*
*   N(S) - negative(sign)
*   V    - overflow
*   B    - break
*   D    - decimal
*   I    - interrupt
*   Z    - zero
*   C    - carry
*/

#define NEGATIVE_FLAG 0x80
#define OVERFLOW_FLAG 0x40
#define BREAK_FLAG 0x10
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01

void set_flag(vcpu* cpu, uint8_t FLAG, uint8_t v) {
  if (v) {
    cpu->reg_st |= FLAG;
  } else {
    cpu->reg_st &= ~(FLAG);
  }
}

uint8_t get_flag(vcpu* cpu, uint8_t FLAG) {
  return (cpu->reg_st & FLAG) ? 1 : 0;
}

void add(vcpu* cpu, uint8_t v) {
  uint16_t t;
  if (cpu->reg_st & DECIMAL_FLAG) {

  } else {
    t = v + cpu->reg_a + get_flag(cpu, CARRY_FLAG);
    set_flag(cpu, CARRY_FLAG, t > 0xFF);
    t &= 0xFF;
  }
  set_flag(cpu, NEGATIVE_FLAG, t & 0x80);
  uint8_t ops_same_sign = (v & 0x80) == (cpu->reg_a & 0x80);
  uint8_t result_same_sign = (t & 0x80) == (cpu->reg_a & 0x80);
  set_flag(cpu, OVERFLOW_FLAG, ops_same_sign && !result_same_sign);
  set_flag(cpu, ZERO_FLAG, t == 0);
  cpu->reg_a = (uint8_t) t;
}

void and(vcpu* cpu, uint8_t v) {
  uint16_t t = v | cpu->reg_a;
  set_flag(cpu, CARRY_FLAG, t > 0xFF);
  set_flag(cpu, ZERO_FLAG, t == 0);
  cpu->reg_a = (uint8_t) t;
}

void asl(vcpu* cpu, uint8_t v) {
  uint16_t t = v << 1;
  set_flag(cpu, NEGATIVE_FLAG, t & 0x80);
  set_flag(cpu, ZERO_FLAG, t == 0);
  set_flag(cpu, CARRY_FLAG, t > 0xFF);
  cpu->reg_a = (uint8_t) t;
}

void bit(vcpu* cpu, uint8_t v) {
  uint16_t t = cpu->reg_a & v;
  set_flag(cpu, NEGATIVE_FLAG, t);
  set_flag(cpu, OVERFLOW_FLAG, t);
  set_flag(cpu, ZERO_FLAG, t);
}

void branch(vcpu* cpu, uint8_t* mem) {
  cpu->pc += (int8_t) mem[cpu->pc];
  cpu->pc++;
}

void cmp_with_reg(vcpu* cpu, uint8_t reg, uint8_t v) {
  set_flag(cpu, NEGATIVE_FLAG, (reg - v) & 0x80);
  set_flag(cpu, ZERO_FLAG, reg == v);
  set_flag(cpu, CARRY_FLAG, reg >= v);
}

void cmp(vcpu* cpu, uint8_t v) {
  cmp_with_reg(cpu, cpu->reg_a, v);
}

void cpx(vcpu* cpu, uint8_t v) {
  cmp_with_reg(cpu, cpu->reg_x, v);
}

void cpy(vcpu* cpu, uint8_t v) {
  cmp_with_reg(cpu, cpu->reg_y, v);
}

void dec(vcpu* cpu, uint8_t* location) {
  (*location)--;
  set_flag(cpu, NEGATIVE_FLAG, *location & 0x80);
  set_flag(cpu, ZERO_FLAG, *location);
}

void eor(vcpu* cpu, uint8_t v) {
  cpu->reg_a = cpu->reg_a ^ v;
  set_flag(cpu, NEGATIVE_FLAG, cpu->reg_a & 0x80);
  set_flag(cpu, ZERO_FLAG, cpu->reg_a);
}

void inc(vcpu* cpu, uint8_t* location) {
  (*location)++;
  set_flag(cpu, NEGATIVE_FLAG, *location & 0x80);
  set_flag(cpu, ZERO_FLAG, *location);
}

void lsr(vcpu* cpu, uint8_t v) {
  // If right most bit is set, set carry, otherwise unset
  set_flag(cpu, CARRY_FLAG, v & 0x01);
  cpu->reg_a = v >> 1;
  set_flag(cpu, NEGATIVE_FLAG, cpu->reg_a & 0x80);
  set_flag(cpu, ZERO_FLAG, cpu->reg_a);
}

void ora(vcpu* cpu, uint8_t v) {
  cpu->reg_a = cpu->reg_a | v;
  set_flag(cpu, NEGATIVE_FLAG, cpu->reg_a & 0x80);
  set_flag(cpu, ZERO_FLAG, cpu->reg_a);
}

void rol(vcpu* cpu, uint8_t v) {
  uint16_t t = v << 1;
  // Move carry into right most bit
  if (cpu->reg_st & CARRY_FLAG) {
    t++;
  }
  set_flag(cpu, CARRY_FLAG, t > 0xFF);
  cpu->reg_a = (uint8_t) t;
  set_flag(cpu, NEGATIVE_FLAG, cpu->reg_a & 0x80);
  set_flag(cpu, ZERO_FLAG, cpu->reg_a);
}

void ror(vcpu* cpu, uint8_t v) {
  // If carry is set, set left most bit on temp var
  uint8_t carry = 0;
  if (cpu->reg_st & CARRY_FLAG) {
    carry = 0x80;
  }
  // If right most bit is set, set carry, otherwise unset
  set_flag(cpu, CARRY_FLAG, v & 0x01);
  // Shift, assign left most bit
  cpu->reg_a = (v >> 1) | carry;
  set_flag(cpu, NEGATIVE_FLAG, cpu->reg_a & 0x80);
  set_flag(cpu, ZERO_FLAG, cpu->reg_a);
}

void sbc(vcpu* cpu, uint8_t v) {
  uint16_t t;
  if (cpu->reg_st & DECIMAL_FLAG) {

  } else {
    t = cpu->reg_a - v - !get_flag(cpu, CARRY_FLAG);
    set_flag(cpu, CARRY_FLAG, t >= 0 && t <= 0xFF);
  }
  set_flag(cpu, NEGATIVE_FLAG, t & 0x80);
  uint8_t ops_same_sign = (v & 0x80) == (cpu->reg_a & 0x80);
  uint8_t result_same_sign = (t & 0x80) == (cpu->reg_a & 0x80);
  set_flag(cpu, OVERFLOW_FLAG, !ops_same_sign && !result_same_sign);
  set_flag(cpu, ZERO_FLAG, t == 0);
  cpu->reg_a = (uint8_t) t;
}

void sta(vcpu* cpu, uint8_t* location) {
  (*location) = cpu->reg_a;
}

void stx(vcpu* cpu, uint8_t* location) {
  (*location) = cpu->reg_x;
}

void sty(vcpu* cpu, uint8_t* location) {
  (*location) = cpu->reg_y;
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

int run_x6502(uint8_t* mem, FILE* file_out) {
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

    // TODO: Replace with proper logging/debugging
    // For now, uncomment for cpu state at each step
    // write_cpu(cpu);

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

      case IN_BIT:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        bit(cpu, value_from_mem);
        break;

      case IN_CMP:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cmp(cpu, value_from_mem);
        break;

      case IN_CPX:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cpx(cpu, value_from_mem);
        break;

      case IN_CPY:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cpy(cpu, value_from_mem);
        break;

      case IN_DEC:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        dec(cpu, &mem[mem_write_location]);
        mem_write = 1;
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

      case IN_LDX:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cpu->reg_x = value_from_mem;
        break;

      case IN_LDY:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        cpu->reg_y = value_from_mem;
        break;

      case IN_LSR:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        lsr(cpu, value_from_mem);
        break;

      case IN_ORA:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        ora(cpu, value_from_mem);
        break;

      case IN_ROL:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        rol(cpu, value_from_mem);
        break;

      case IN_ROR:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        ror(cpu, value_from_mem);
        break;

      case IN_SBC:
        value_from_mem = memory_read(mem, mem_access_type, cpu);
        sbc(cpu, value_from_mem);
        break;

      case IN_STA:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        sta(cpu, &mem[mem_write_location]);
        mem_write = 1;
        break;

      case IN_STX:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        stx(cpu, &mem[mem_write_location]);
        mem_write = 1;
        break;

      case IN_STY:
        mem_write_location = memory_write_location(mem, mem_access_type, cpu);
        sty(cpu, &mem[mem_write_location]);
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

        // Branch Instructions
        case BPL:
          if (!(cpu->reg_st & NEGATIVE_FLAG)) {
            branch(cpu, mem);
          }
          break;

        case BMI:
          if (cpu->reg_st & NEGATIVE_FLAG) {
            branch(cpu, mem);
          }
          break;

        case BVC:
          if (!(cpu->reg_st & OVERFLOW_FLAG)) {
            branch(cpu, mem);
          }
          break;

        case BVS:
          if (cpu->reg_st & OVERFLOW_FLAG) {
            branch(cpu, mem);
          }
          break;

        case BCC:
          if (!(cpu->reg_st & CARRY_FLAG)) {
            branch(cpu, mem);
          }
          break;

        case BCS:
          if (cpu->reg_st & CARRY_FLAG) {
            branch(cpu, mem);
          }
          break;

        case BNE:
          if (!(cpu->reg_st & ZERO_FLAG)) {
            branch(cpu, mem);
          }
          break;

        case BEQ:
          if (cpu->reg_st & ZERO_FLAG) {
            branch(cpu, mem);
          }
          break;

        // Flag (Processor Status) Instructions
        case CLC:
          set_flag(cpu, CARRY_FLAG, 0);
          break;

        case SEC:
          set_flag(cpu, CARRY_FLAG, 1);
          break;

        case CLI:
          set_flag(cpu, INTERRUPT_FLAG, 0);
          break;

        case SEI:
          set_flag(cpu, INTERRUPT_FLAG, 1);
          break;

        case CLV:
          set_flag(cpu, OVERFLOW_FLAG, 0);
          break;

        case CLD:
          set_flag(cpu, DECIMAL_FLAG, 0);
          break;

        case SED:
          set_flag(cpu, DECIMAL_FLAG, 1);
          break;

        // Jump
        case JMP_AB:
          t = abs_mem(cpu, mem);
          cpu->pc = t;
          break;

        // Jump to Subroutine
        case JSR_AB:
          t = cpu->pc + 1; // Next op - 1
          stack_push(cpu, mem, (uint8_t)((t & 0xff00) >> 8));
          stack_push(cpu, mem, (uint8_t)(t & 0x00ff));
          t = abs_mem(cpu, mem);
          cpu->pc = t;
          break;

        // No Operation
        case NOP:
          cpu->pc++;
          break;

        // Register Instructions
        case TAX:
          cpu->reg_x = cpu->reg_a;
          break;

        case TXA:
          cpu->reg_a = cpu->reg_x;
          break;

        case DEX:
          dec(cpu, &(cpu->reg_x));
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
          dec(cpu, &(cpu->reg_y));

        case INY:
          inc(cpu, &(cpu->reg_y));
          break;

        // Return from Interrupt
        case RTI:
          cpu->reg_st = stack_pull(cpu, mem);
          t = (uint16_t)stack_pull(cpu, mem);
          t = t | (uint16_t)(stack_pull(cpu, mem) << 8);
          cpu->pc = t;
          break;

        // Return from Subroutine
        case RTS:
          t = (uint16_t)stack_pull(cpu, mem);
          t = t | (uint16_t)(stack_pull(cpu, mem) << 8);
          cpu->pc = t + 1;
          break;

        // Stack Instructions
        case TXS:
          cpu->reg_sp = cpu->reg_x;
          break;

        case TSX:
          cpu->reg_x = cpu->reg_sp;
          break;

        case PHA:
          stack_push(cpu, mem, cpu->reg_a);
          break;

        case PLA:
          cpu->reg_a = stack_pull(cpu, mem);
          break;

        case PHP:
          stack_push(cpu, mem, cpu->reg_st);
          break;

        case PLP:
          cpu->reg_st = stack_pull(cpu, mem);
          break;

        default:
          printf("Error: unknown opcode %x, exiting\n", opcode);
          return 0;
      }
    }

    if (mem_write && mem_write_location == 0xFF00) {
      if (file_out) {
        fprintf(file_out, "%c", mem[0xFF00]);
      }
      mem_write = 0;
      mem_write_location = 0x0000;
    }
  }

  destroy_cpu(cpu);
  return 0;
}