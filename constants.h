#ifndef CONSTANTS
#define CONSTANTS

enum enum_opcodes {
  ADC_IM  = 0x69,
  ADC_ZE  = 0x65,
  ADC_ZX  = 0x75,
  ADC_AB  = 0x6D,
  ADC_AX  = 0x7D,
  ADC_AY  = 0x79,
  ADC_IX  = 0x61,
  ADC_IY  = 0x71,

  AND_IM  = 0x29,
  AND_ZE  = 0x25,
  AND_ZX  = 0x35,
  AND_AB  = 0x2D,
  AND_AX  = 0x3D,
  AND_AY  = 0x39,
  AND_IX  = 0x21,
  AND_IY  = 0x31,

  ASL_IM  = 0x0A,
  ASL_ZE  = 0x06,
  ASL_ZX  = 0x16,
  ASL_AB  = 0x0E,
  ASL_AX  = 0x1E,

  BIT_ZE  = 0x24,
  BIT_AB  = 0x2C,

  BPL     = 0x10,
  BMI     = 0x30,
  BVC     = 0x50,
  BVS     = 0x70,
  BCC     = 0x90,
  BCS     = 0xB0,
  BNE     = 0xD0,
  BEQ     = 0xF0,

  BRK     = 0x00,

  CMP_IM  = 0xC9,
  CMP_ZE  = 0xC5,
  CMP_ZX  = 0xD5,
  CMP_AB  = 0xCD,
  CMP_AX  = 0xDD,
  CMP_AY  = 0xD9,
  CMP_IX  = 0xC1,
  CMP_IY  = 0xD1,

  CPX_IM  = 0xE0,
  CPX_ZE  = 0xE4,
  CPX_AB  = 0xEC,

  CPY_IM  = 0xC0,
  CPY_ZE  = 0xC4,
  CPY_AB  = 0xCC,

  DEC_ZE  = 0xC6,
  DEC_ZX  = 0xD6,
  DEC_AB  = 0xCE,
  DEC_AX  = 0xDE,

  EOR_IM  = 0x49,
  EOR_ZE  = 0x45,
  EOR_ZX  = 0x55,
  EOR_AB  = 0x4D,
  EOR_AX  = 0x5D,
  EOR_AY  = 0x59,
  EOR_IX  = 0x41,
  EOR_IY  = 0x51,

  CLC     = 0x18,
  SEC     = 0x38,
  CLI     = 0x58,
  SEI     = 0x78,
  CLV     = 0xB8,
  CLD     = 0xD8,
  SED     = 0xF8,

  INC_ZE  = 0xE6,
  INC_ZX  = 0xF6,
  INC_AB  = 0xEE,
  INC_AX  = 0xFE,

  JMP_AB  = 0x4C,
  JMP_IN  = 0x6C,

  JSR_AB  = 0x20,

  LDA_IM  = 0xA9,
  LDA_ZE  = 0xA5,
  LDA_ZX  = 0xB5,
  LDA_AB  = 0xAD,
  LDA_AX  = 0xBD,
  LDA_AY  = 0xB9,
  LDA_IX  = 0xA1,
  LDA_IY  = 0xB1,

  LDX_IM  = 0xA2,
  LDX_ZE  = 0xA6,
  LDX_ZY  = 0xB6,
  LDX_AB  = 0xAE,
  LDX_AY  = 0xBE,

  LDY_IM  = 0xA0,
  LDY_ZE  = 0xA4,
  LDY_ZY  = 0xB4,
  LDY_AB  = 0xAC,
  LDY_AY  = 0xBC,

  LSR_A   = 0x4A,
  LSR_ZE  = 0x46,
  LSR_ZX  = 0x56,
  LSR_AB  = 0x4E,
  LSR_AX  = 0x5E,

  NOP     = 0xEA,

  ORA_IM  = 0x09,
  ORA_ZE  = 0x05,
  ORA_ZX  = 0x15,
  ORA_AB  = 0x0D,
  ORA_AX  = 0x1D,
  ORA_AY  = 0x19,
  ORA_IX  = 0x01,
  ORA_IY  = 0x11,

  TAX     = 0xAA,
  TXA     = 0x8A,
  DEX     = 0xCA,
  INX     = 0xE8,
  TAY     = 0xA8,
  TYA     = 0x98,
  DEY     = 0x88,
  INY     = 0xC8,

  ROL_A   = 0x2A,
  ROL_ZE  = 0x26,
  ROL_ZX  = 0x36,
  ROL_AB  = 0x2E,
  ROL_AX  = 0x3E,

  ROR_A   = 0x6A,
  ROR_ZE  = 0x66,
  ROR_ZX  = 0x76,
  ROR_AB  = 0x6E,
  ROR_AX  = 0x7E,

  RTI     = 0x40,

  RTS     = 0x60,

  SBC_IM  = 0xE9,
  SBC_ZE  = 0xE5,
  SBC_ZX  = 0xF5,
  SBC_AB  = 0xED,
  SBC_AX  = 0xFD,
  SBC_AY  = 0xF9,
  SBC_IX  = 0xE1,
  SBC_IY  = 0xF1,

  STA_ZE  = 0x85,
  STA_ZX  = 0x95,
  STA_AB  = 0x8D,
  STA_AX  = 0x9D,
  STA_AY  = 0x99,
  STA_IX  = 0x81,
  STA_IY  = 0x91,

  TXS     = 0x9A,
  TSX     = 0xBA,
  PHA     = 0x48,
  PLA     = 0x68,
  PHP     = 0x08,
  PLP     = 0x28,

  STX_ZE  = 0x86,
  STX_ZY  = 0x96,
  STX_AB  = 0x8E,

  STY_ZE  = 0x84,
  STY_ZY  = 0x94,
  STY_AB  = 0x8C,

  EXT     = 0xFF
};

enum enum_instructions {
  IN_NULL,
  IN_ADC,
  IN_AND,
  IN_ASL,
  IN_BIT,
  IN_CMP,
  IN_CPX,
  IN_CPY,
  IN_DEC,
  IN_EOR,
  IN_INC,
  IN_LDA,
  IN_LDX,
  IN_LDY,
  IN_LSR,
  IN_ORA,
  IN_ROL,
  IN_ROR,
  IN_SBC,
  IN_STA,
  IN_STX,
  IN_STY
};

enum enum_memory_access_types {
  ME_NULL,
  ME_A,
  ME_IM,
  ME_ZE,
  ME_ZX,
  ME_ZY,
  ME_AB,
  ME_AX,
  ME_AY,
  ME_IX,
  ME_IY
};

#endif