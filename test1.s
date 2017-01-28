; Puts ASCII zero into acc, writes
LDA       #$30
STA       $FF00

; Do a bitwise AND on 1 and 2 (should be 3), add to 0x30 to get ascii 3
LDA       #$01
STA       $FF10
LDA       #$02
AND       $FF10
ADC       #$30
STA       $FF00

; Perform ASL on 1 and write result + 0x30, should see 2, 4, 8
LDA       #$01
STA       $FF10
ASL       $FF10
STA       $FF10
ADC       #$30
STA       $FF00
LDA       $FF10
ASL
STA       $FF10
ADC       #$30
STA       $FF00
LDA       $FF10
ASL
ADC       #$30
STA       $FF00

; EOR 0xF0 with 0xE1 should give me 0xb, add to ascii 'A', should be 'R'
LDA       #$F0
STA       $FF10
LDA       #$E1
EOR       $FF10
ADC       #$41
STA       $FF00

; ORA 0x01 and 0x02 should give me 0x03, add to ascii '0', for '3'
LDA       #$01
STA       $FF10
LDA       #$02
ORA       $FF10
ADC       #$30
STA       $FF00
