#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS

#include "constants.h"
#include "cpu.h"

uint16_t memory_location_inc_pc(uint8_t* mem, uint8_t memory_type, vcpu* cpu);

// Return memory location by reading two consecutive bytes in memory to get
// uint16_t address
uint8_t memory_read(uint8_t* mem, uint8_t memory_type, vcpu* cpu) {
  if (memory_type == ME_A) {
    return cpu->reg_a;
  }
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
    case ME_IM:
      location = cpu->pc++;
      break;

    case ME_ZE:
      location = (uint16_t)mem[cpu->pc++];
      break;

    case ME_ZX:
      location = (uint16_t)mem[cpu->pc++] + cpu->reg_x;
      location = location & 0x00FF;
      break;

    case ME_AB:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      break;

    case ME_AX:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      location += cpu->reg_x;
      break;

    case ME_AY:
      location = mem[cpu->pc] | mem[cpu->pc+1] << 8;
      cpu->pc += 2;
      location += cpu->reg_y;
      break;

    case ME_IX:
      indirect = (uint16_t)mem[cpu->pc++] + cpu->reg_x;
      location = mem[indirect] | mem[indirect+1] << 8;
      break;

    case ME_IY:
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

uint8_t* get_memory_lookup() {
  uint8_t* t = (uint8_t*) malloc(sizeof(uint8_t) * 256);
  int i = 0;
  for (; i < 256; i++) t[i] = ME_NULL;

  t[ADC_IM] = ME_IM;
  t[ADC_ZE] = ME_ZE;
  t[ADC_ZX] = ME_ZX;
  t[ADC_AB] = ME_AB;
  t[ADC_AX] = ME_AX;
  t[ADC_AY] = ME_AY;
  t[ADC_IX] = ME_IX;
  t[ADC_IY] = ME_IY;

  t[AND_IM] = ME_IM;
  t[AND_ZE] = ME_ZE;
  t[AND_ZX] = ME_ZX;
  t[AND_AB] = ME_AB;
  t[AND_AX] = ME_AX;
  t[AND_AY] = ME_AY;
  t[AND_IX] = ME_IX;
  t[AND_IY] = ME_IY;

  t[ASL_A]  = ME_A;
  t[ASL_ZE] = ME_ZE;
  t[ASL_ZX] = ME_ZX;
  t[ASL_AB] = ME_AB;
  t[ASL_AX] = ME_AX;

  t[CMP_IM] = ME_IM;
  t[CMP_ZE] = ME_ZE;
  t[CMP_ZX] = ME_ZX;
  t[CMP_AB] = ME_AB;
  t[CMP_AX] = ME_AX;
  t[CMP_AY] = ME_AY;
  t[CMP_IX] = ME_IX;
  t[CMP_IY] = ME_IY;

  t[CPX_IM] = ME_IM;
  t[CPX_ZE] = ME_ZE;
  t[CPX_AB] = ME_AB;

  t[CPY_IM] = ME_IM;
  t[CPY_ZE] = ME_ZE;
  t[CPY_AB] = ME_AB;

  t[DEC_ZE] = ME_ZE;
  t[DEC_ZX] = ME_ZX;
  t[DEC_AB] = ME_AB;
  t[DEC_AX] = ME_AX;

  t[EOR_IM] = ME_IM;
  t[EOR_ZE] = ME_ZE;
  t[EOR_ZX] = ME_ZX;
  t[EOR_AB] = ME_AB;
  t[EOR_AX] = ME_AX;
  t[EOR_AY] = ME_AY;
  t[EOR_IX] = ME_IX;
  t[EOR_IY] = ME_IY;

  t[INC_ZE] = ME_ZE;
  t[INC_ZX] = ME_ZX;
  t[INC_AB] = ME_AB;
  t[INC_AX] = ME_AX;

  t[LDA_IM] = ME_IM;
  t[LDA_ZE] = ME_ZE;
  t[LDA_ZX] = ME_ZX;
  t[LDA_AB] = ME_AB;
  t[LDA_AX] = ME_AX;
  t[LDA_AY] = ME_AY;
  t[LDA_IX] = ME_IX;
  t[LDA_IY] = ME_IY;

  t[LDX_IM] = ME_IM;
  t[LDX_ZE] = ME_ZE;
  t[LDX_ZY] = ME_ZY;
  t[LDX_AB] = ME_AB;
  t[LDX_AY] = ME_AY;

  t[LDY_IM] = ME_IM;
  t[LDY_ZE] = ME_ZE;
  t[LDY_ZY] = ME_ZY;
  t[LDY_AB] = ME_AB;
  t[LDY_AY] = ME_AY;

  t[LSR_A] = ME_A;
  t[LSR_ZE] = ME_ZE;
  t[LSR_ZX] = ME_ZX;
  t[LSR_AB] = ME_AB;
  t[LSR_AX] = ME_AX;

  t[ORA_IM] = ME_IM;
  t[ORA_ZE] = ME_ZE;
  t[ORA_ZX] = ME_ZX;
  t[ORA_AB] = ME_AB;
  t[ORA_AX] = ME_AX;
  t[ORA_AY] = ME_AY;
  t[ORA_IX] = ME_IX;
  t[ORA_IY] = ME_IY;

  t[ROL_A] = ME_A;
  t[ROL_ZE] = ME_ZE;
  t[ROL_ZX] = ME_ZX;
  t[ROL_AB] = ME_AB;
  t[ROL_AX] = ME_AX;

  t[ROR_A] = ME_A;
  t[ROR_ZE] = ME_ZE;
  t[ROR_ZX] = ME_ZX;
  t[ROR_AB] = ME_AB;
  t[ROR_AX] = ME_AX;

  t[SBC_IM] = ME_IM;
  t[SBC_ZE] = ME_ZE;
  t[SBC_ZX] = ME_ZX;
  t[SBC_AB] = ME_AB;
  t[SBC_AX] = ME_AX;
  t[SBC_AY] = ME_AY;
  t[SBC_IX] = ME_IX;
  t[SBC_IY] = ME_IY;

  t[STA_ZE] = ME_ZE;
  t[STA_ZX] = ME_ZX;
  t[STA_AB] = ME_AB;
  t[STA_AX] = ME_AX;
  t[STA_AY] = ME_AY;
  t[STA_IX] = ME_IX;
  t[STA_IY] = ME_IY;

  t[STX_ZE] = ME_ZE;
  t[STX_ZY] = ME_ZY;
  t[STX_AB] = ME_AB;

  t[STY_ZE] = ME_ZE;
  t[STY_ZY] = ME_ZY;
  t[STY_AB] = ME_AB;

  return t;
}

#endif