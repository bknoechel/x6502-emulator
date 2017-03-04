#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include "constants.h"

uint8_t * get_instruction_lookup() {
  uint8_t* t = (uint8_t*) malloc(sizeof(uint8_t) * 256);
  int i = 0;
  for (; i < 256; i++) t[i] = IN_NULL;

  t[ADC_IM] = IN_ADC;
  t[ADC_ZE] = IN_ADC;
  t[ADC_ZX] = IN_ADC;
  t[ADC_AB] = IN_ADC;
  t[ADC_AX] = IN_ADC;
  t[ADC_AY] = IN_ADC;
  t[ADC_IX] = IN_ADC;
  t[ADC_IY] = IN_ADC;

  t[AND_IM] = IN_AND;
  t[AND_ZE] = IN_AND;
  t[AND_ZX] = IN_AND;
  t[AND_AB] = IN_AND;
  t[AND_AX] = IN_AND;
  t[AND_AY] = IN_AND;
  t[AND_IX] = IN_AND;
  t[AND_IY] = IN_AND;

  t[ASL_A] = IN_ASL;
  t[ASL_ZE] = IN_ASL;
  t[ASL_ZX] = IN_ASL;
  t[ASL_AB] = IN_ASL;
  t[ASL_AX] = IN_ASL;

  t[BIT_ZE] = IN_BIT;
  t[BIT_AB] = IN_BIT;

  t[CMP_IM] = IN_CMP;
  t[CMP_ZE] = IN_CMP;
  t[CMP_ZX] = IN_CMP;
  t[CMP_AB] = IN_CMP;
  t[CMP_AX] = IN_CMP;
  t[CMP_AY] = IN_CMP;
  t[CMP_IX] = IN_CMP;
  t[CMP_IY] = IN_CMP;

  t[CPX_IM] = IN_CPX;
  t[CPX_ZE] = IN_CPX;
  t[CPX_AB] = IN_CPX;

  t[CPY_IM] = IN_CPY;
  t[CPY_ZE] = IN_CPY;
  t[CPY_AB] = IN_CPY;

  t[DEC_ZE] = IN_DEC;
  t[DEC_ZX] = IN_DEC;
  t[DEC_AB] = IN_DEC;
  t[DEC_AX] = IN_DEC;

  t[EOR_IM] = IN_EOR;
  t[EOR_ZE] = IN_EOR;
  t[EOR_ZX] = IN_EOR;
  t[EOR_AB] = IN_EOR;
  t[EOR_AX] = IN_EOR;
  t[EOR_AY] = IN_EOR;
  t[EOR_IX] = IN_EOR;
  t[EOR_IY] = IN_EOR;

  t[INC_ZE] = IN_INC;
  t[INC_ZX] = IN_INC;
  t[INC_AB] = IN_INC;
  t[INC_AX] = IN_INC;

  t[LDA_IM] = IN_LDA;
  t[LDA_ZE] = IN_LDA;
  t[LDA_ZX] = IN_LDA;
  t[LDA_AB] = IN_LDA;
  t[LDA_AX] = IN_LDA;
  t[LDA_AY] = IN_LDA;
  t[LDA_IX] = IN_LDA;
  t[LDA_IY] = IN_LDA;

  t[LDX_IM] = IN_LDX;
  t[LDX_ZE] = IN_LDX;
  t[LDX_ZY] = IN_LDX;
  t[LDX_AB] = IN_LDX;
  t[LDX_AY] = IN_LDX;

  t[LDY_IM] = IN_LDY;
  t[LDY_ZE] = IN_LDY;
  t[LDY_ZY] = IN_LDY;
  t[LDY_AB] = IN_LDY;
  t[LDY_AX] = IN_LDY;

  t[LSR_A] = IN_LSR;
  t[LSR_ZE] = IN_LSR;
  t[LSR_ZX] = IN_LSR;
  t[LSR_AB] = IN_LSR;
  t[LSR_AX] = IN_LSR;

  t[ORA_IM] = IN_ORA;
  t[ORA_ZE] = IN_ORA;
  t[ORA_ZX] = IN_ORA;
  t[ORA_AB] = IN_ORA;
  t[ORA_AX] = IN_ORA;
  t[ORA_AY] = IN_ORA;
  t[ORA_IX] = IN_ORA;
  t[ORA_IY] = IN_ORA;

  t[ROL_A] = IN_ROL;
  t[ROL_ZE] = IN_ROL;
  t[ROL_ZX] = IN_ROL;
  t[ROL_AB] = IN_ROL;
  t[ROL_AX] = IN_ROL;

  t[ROR_A] = IN_ROR;
  t[ROR_ZE] = IN_ROR;
  t[ROR_ZX] = IN_ROR;
  t[ROR_AB] = IN_ROR;
  t[ROR_AX] = IN_ROR;

  t[SBC_IM] = IN_SBC;
  t[SBC_ZE] = IN_SBC;
  t[SBC_ZX] = IN_SBC;
  t[SBC_AB] = IN_SBC;
  t[SBC_AX] = IN_SBC;
  t[SBC_AY] = IN_SBC;
  t[SBC_IX] = IN_SBC;
  t[SBC_IY] = IN_SBC;

  t[STA_ZE] = IN_STA;
  t[STA_ZX] = IN_STA;
  t[STA_AB] = IN_STA;
  t[STA_AX] = IN_STA;
  t[STA_AY] = IN_STA;
  t[STA_IX] = IN_STA;
  t[STA_IY] = IN_STA;

  t[STX_ZE] = IN_STX;
  t[STX_ZY] = IN_STX;
  t[STX_AB] = IN_STX;

  t[STY_ZE] = IN_STY;
  t[STY_ZY] = IN_STY;
  t[STY_AB] = IN_STY;

  return t;
}

#endif