#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS

#include "opcodes.h"
#include "cpu.h"

uint16_t memory_location_inc_pc(uint8_t* mem, uint8_t memory_type, vcpu* cpu);

// Return memory location by reading two consecutive bytes in memory to get
// uint16_t address

#define MEM_TYPE_IM  0x01
#define MEM_TYPE_ZE  0x02
#define MEM_TYPE_ZX  0x03
#define MEM_TYPE_AB  0x04
#define MEM_TYPE_AX  0x05
#define MEM_TYPE_AY  0x06
#define MEM_TYPE_IX  0x07
#define MEM_TYPE_IY  0x08

uint8_t get_memory_access_type(uint8_t opcode) {
  switch(opcode) {
    case ADC_IM:
      return MEM_TYPE_IM;

    case ADC_ZE:
    case INC_ZE:
    case STA_ZE:
      return MEM_TYPE_ZE;

    case ADC_ZX:
    case INC_ZX:
    case STA_ZX:
      return MEM_TYPE_ZX;

    case ADC_AB:
    case INC_AB:
    case STA_AB:
      return MEM_TYPE_AB;

    case ADC_AX:
    case INC_AX:
    case STA_AX:
      return MEM_TYPE_AX;

    case ADC_AY:
    case STA_AY:
      return MEM_TYPE_AY;

    case ADC_IX:
    case STA_IX:
      return MEM_TYPE_IX;

    case ADC_IY:
    case STA_IY:
      return MEM_TYPE_IY;

    default:
      return 0;
  }
}

uint8_t memory_read(uint8_t* mem, uint8_t memory_type, vcpu* cpu) {
  uint16_t location = memory_location_inc_pc(mem, memory_type, cpu);
  return mem[location];
}

uint16_t memory_write_location(uint8_t* mem, uint8_t memory_type, vcpu* cpu) {
  uint16_t location = memory_location_inc_pc(mem, memory_type, cpu);
  return location;
}

uint16_t memory_location_inc_pc(uint8_t* mem, uint8_t memory_type, vcpu* cpu) {
  uint16_t location, indirect;
  switch(memory_type) {
    case MEM_TYPE_IM:
      location = cpu->pc++;
      break;

    case MEM_TYPE_ZE:
      location = (uint16_t)mem[cpu->pc++];
      break;

    case MEM_TYPE_ZX:
      location = (uint16_t)mem[cpu->pc++] + cpu->reg_x;
      location = location & 0x00FF;
      break;

    case MEM_TYPE_AB:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      break;

    case MEM_TYPE_AX:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      location += cpu->reg_x;
      break;

    case MEM_TYPE_AY:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      location += cpu->reg_y;
      break;

    case MEM_TYPE_IX:
      indirect = (uint16_t)mem[cpu->pc++] + cpu->reg_x;
      location = mem[indirect] | mem[indirect+1] << 8;
      break;

    case MEM_TYPE_IY:
      indirect = (uint16_t)mem[cpu->pc++] + cpu->reg_y;
      location = mem[indirect] | mem[indirect+1] << 8;
      break;
  }

  return location;
}

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

#endif