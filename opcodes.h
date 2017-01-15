#ifndef OPCODES
#define OPCODES

#define NOP     0xEA

#define ADC_IM  0x69

#define EXT     0xFF
#define LDA_IM  0xA9
#define STA     0x8D

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

#define CMP_IM  0xC9
#define CMP_AB  0xCD

#define INC_AB  0xEE

#define TAX     0xAA
#define TXA     0x8A
#define DEX     0xCA
#define INX     0xE8
#define TAY     0xA8
#define TYA     0x98
#define DEY     0x88
#define INY     0xC8


#endif