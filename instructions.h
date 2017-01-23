#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include "opcodes.h"



#define ADC       0x01
#define INC       0x02
#define STA       0x03




uint8_t get_instruction(uint8_t opcode) {
  switch (opcode) {
    case ADC_IM:
    case ADC_ZE:
    case ADC_ZX:
    case ADC_AB:
    case ADC_AX:
    case ADC_AY:
    case ADC_IX:
    case ADC_IY:
      return ADC;

    case INC_ZE:
    case INC_ZX:
    case INC_AB:
    case INC_AX:
      return INC;

    case STA_ZE:
    case STA_ZX:
    case STA_AB:
    case STA_AX:
    case STA_AY:
    case STA_IX:
    case STA_IY:
      return STA;

    default:
      return 0;
  }
}



#endif