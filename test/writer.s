

  LDX  #$01
  JSR  print_hex_x

  CLC
  BCC  exit

print_hex_x:
  TXA
  ORA  #$0F
  CMP  #$09
  BPL  exit
  ADC  #$30  ; ascii zero, then add x 0 - 9
  STA  $FF00
  RTS

exit:
  LDA  #$0A
  STA  $FF00
  NOP
