        LDA       #$02
        STA       $FF02
        LDA       #$41    ; set 'A' to acc
        ADC       #$01    ; add 1
        INX               ; inc x
        TXA               ; move to A
        ADC       #$41    ; add 'A' to 1 B
        STA       $FF00
        LDA       #$5A    ; Put 'Z' into acc
loop:
        INC       $FF00   ; inc 'B'
        CMP       $FF00   ; compare to acc
        BNE       loop    ; branch if not equal to acc
        STA       $FF00
        LDA       #$0A
        STA       $FF00

