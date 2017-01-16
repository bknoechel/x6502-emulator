
  JSR init
  JSR add
  ADC   #$30
  STA   $FF00
  JSR end

init:
  LDX   #$05
  LDY   #$03
  RTS

add:
  TXA
  STA   $FF02
  TYA
  ADC   $FF02
  RTS

end:
  NOP
  NOP