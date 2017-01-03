        LDA       #$02 
        STA       $FF02
        LDA       #$41
        ADC       #$01
        INX
        TXA
        ADC       #$41
        STA       $FF00
        STA       $FF00
        LDA       #$0A
        STA       $FF00

