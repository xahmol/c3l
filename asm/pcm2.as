;
; Copyright (c) Steven P. Goldsmith. All rights reserved.
;
; Assembled with HI-TECH C 3.09 (CP/M-80) ZAS.
;
; Play 2 bit pulse wave encoded data using SID master volume.
;

global  _playPcm2Sid
psect   data

; Return address

return:

defw    0

; Sample amplitude can be 1-3.

amp:

defw    0

start:

defw    0

psect   text

_playPcm2Sid:
        pop     hl              ; Return address
        ld      (return),hl     ; Save return address
        pop     bc              ; Sample start address
        ld      (start), bc     ; Save sample start address
        pop     de              ; Sample length
        pop     hl              ; Sample amplitude        
        ld      (amp),hl        ; Save sample amplitude
        push    hl
        push    de
        push    bc
        ld      hl,(return)     ; Get saved return address        
        push    hl
        push    ix              ; Preserve ix        
        ex      de,hl           ; Swap de and hl (sample length)
        ld      ix,(start)                   
        rep1:                   ; Repeat
        ld      d,(ix+0)        ; d = Sample byte
        ld      e,04h           ; e = 4 bit pairs to count
        rep2:                   ; Repeat
        ld      bc,0dd0dh       ; bc = CIA 2 ICR
        rep3:                   ; Repeat
        in      a,(c)           ; a = CIA 2 ICR value
        bit     0,a             ; a = 0
        jr      z,rep3          ; Until interrupt flag set        
        ld      bc,(amp)        ; bc = amp        
        ld      a,00h           ; a = 0
        rlc     d               ; Get first sample bit
        rla                     ; Shift carry bit to a
        rlc     d               ; Get second sample bit
        rep4:                   ; Repeat
        rla                     ; Shift carry bit to a
        dec     c               ; c = c-1
        jr      nz,rep4         ; Until c = 0        
        ld      bc,0d418h       ; bc = SID volume address
        out     (c),a           ; Set volume
        dec     e               ; e = e-1
        jr      nz,rep2         ; Until e = 0
        inc     ix              ; ix = ix+1
        dec     l               ; l = l-1
        jr      nz,rep1         ; Until l = 0
        dec     h               ; h = h-1
        jr      nz,rep1         ; Until h = 0
        pop     ix              ; Restore ix
        ret
