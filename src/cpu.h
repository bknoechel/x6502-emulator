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

// TODO: Remove this and find a better way to debug
void write_cpu(vcpu* cpu) {
  int i = 0;
  printf("PC: %x, X: %x, Y: %x, AC: %x, SP: %x, ST:%x, ST:", cpu->pc, cpu->reg_x, cpu->reg_y, cpu->reg_a, cpu->reg_sp, cpu->reg_st);
  for (; i < 8; i++) {
    char c = cpu->reg_st & (1 << (7 - i)) ? '1' : '0';
    printf("%c", c);
  }
  printf("\n");
}

void destroy_cpu(vcpu* ptr) {
  free(ptr);
}

#endif