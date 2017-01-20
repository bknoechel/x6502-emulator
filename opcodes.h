#ifndef OPCODES
#define OPCODES

#define NOP     0xEA

#define ADC_IM  0x69
#define ADC_ZE  0x65
#define ADC_ZX  0x75
#define ADC_AB  0x6D
#define ADC_AX  0x7D
#define ADC_AY  0x79

#define EXT     0xFF
#define LDA_IM  0xA9
#define STA_ZE  0x85
#define STA_ZX  0x95
#define STA_AB  0x8D
#define STA_AX  0x9D
#define STA_AY  0x99

#define BPL     0x10
#define BMI     0x30
#define BVC     0x50
#define BVS     0x70
#define BCC     0x90
#define BCS     0xB0
#define BNE     0xD0
#define BEQ     0xF0

#define JMP_AB  0x4C
#define JSR     0x20
#define RTS     0x60

#define TXS     0x9A
#define TSX     0xBA
#define PHA     0x48
#define PLA     0x68
#define PHP     0x08
#define PLP     0x28

#define CMP_IM  0xC9
#define CMP_AB  0xCD

#define INC_AB  0xEE

#define LDX_IM  0xA2
#define LDX_AB  0xAE
#define LDY_IM  0xA0
#define LDY_AB  0xAC

#define TAX     0xAA
#define TXA     0x8A
#define DEX     0xCA
#define INX     0xE8
#define TAY     0xA8
#define TYA     0x98
#define DEY     0x88
#define INY     0xC8


#endif