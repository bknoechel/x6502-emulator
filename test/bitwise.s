
  ; Goto start
  CLC
  BCC  start

print_status:
  PHP
  TSX
  INX
  LDY       $0100, X
  TYA
  TAX
  JSR       print_hex_x
  PLP
  RTS

  ; prints out hex in x register
print_hex_x:
  ; print 0x
  LDA       #$30
  STA       $FF00
  LDA       #$78
  STA       $FF00
  ; put upper four bits into acc as lower bits
  TXA
  LSR
  LSR
  LSR
  LSR

  ; add 0x30 to acc to get to '0' and then add acc
  ; if greater or equal to 10, add 0x07 to get to 'A'
  ; NOTE 0x03 is '0', add 0x11 to get to 'A', subtract 10 because we add value
  ; in acc that is greater than 10, result is, add 0x06
  CMP       #$0A
  BCC       _print_hex_add30
  ADC       #$06
_print_hex_add30:
  ADC       #$30
  STA       $FF00
  TXA
  AND       #$0F
  CMP       #$0A
  BCC       _print_hex_add30_2
  ADC       #$06
_print_hex_add30_2:
  ADC       #$30
  STA       $FF00
  ; add newline, return
  LDA       #$0A
  STA       $FF00
  RTS


start:
  ; Bitwise AND
  LDA       #$FF
  AND       #$83
  TAX
  JSR       print_hex_x ; 0x83
  LDA       #$83
  AND       #$03
  TAX
  JSR       print_hex_x ; 0x03
  LDA       #$03
  AND       #$00
  TAX
  JSR       print_hex_x ; 0x00
  LDA       #$0A
  STA       $FF00

  ; Arithmetic Shift left
  LDA       #$01
  ASL
  TAX
  JSR       print_hex_x ; 0x02
  LDA       #$02
  ASL
  TAX
  JSR       print_hex_x ; 0x04
  LDA       #$FF
  ASL
  TAX
  JSR       print_hex_x ; 0xFE
  LDA       #$FF
  ASL
  TAX
  JSR       print_status ; 0x81
  LDA       #$0A
  STA       $FF00

  ; Bitwise Exclusive OR
  LDA       #$FF
  EOR       #$0F
  TAX
  JSR       print_hex_x ; 0xF0
  LDA       #$00
  EOR       #$0F
  TAX
  JSR       print_hex_x ; 0x0F
  LDA       #$0A
  STA       $FF00

  ; Logical Shift Right
  LDA       #$04
  LSR
  TAX
  JSR       print_hex_x ; 0x02
  LDA       #$04
  LSR
  LSR
  TAX
  JSR       print_hex_x ; 0x01
  LDA       #$04
  LSR
  LSR
  LSR
  TAX
  JSR       print_hex_x ; 0x00
  LDA       #$04
  LSR
  LSR
  LSR
  LSR
  TAX
  JSR       print_hex_x ; 0x00
  LDA       #$0A
  STA       $FF00

  ; Bitwise OR
  LDA       #$FF
  ORA       #$0F
  TAX
  JSR       print_hex_x ; 0xFF
  LDA       #$00
  ORA       #$0F
  TAX
  JSR       print_hex_x ; 0x0F
  LDA       #$0A
  STA       $FF00

  ; Rotate Left
  LDA       #$01
  ROL
  TAX
  JSR       print_hex_x ; 0x02
  LDA       #$FF
  ROL
  ROL
  ROL
  ROL
  ROL
  TAX
  JSR       print_hex_x ; 0xEF
  LDA       #$0A
  STA       $FF00

  ; Rotate Right
  LDA       #$01
  ROR
  TAX
  JSR       print_hex_x ; 0x00
  LDA       #$FF
  ROR
  ROR
  ROR
  ROR
  ROR
  TAX
  JSR       print_hex_x ; 0xF7
  LDA       #$0A
  STA       $FF00
