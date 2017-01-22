#ifndef CPU
#define CPU

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

#endif