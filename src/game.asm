ORG 0

%define SCREEN_WIDTH 320
%define SCREEN_HEIGHT 180

%define VRAM_DWORD_START 124961600
%define VRAM_DWORD_END 125019200

%define UNUSED_ADDRESS_MAX 62480800

%define COLOR_WHITE 0xFFFFFFFF
%define PORT_KBD_IN 0x01

%define PORT_PIT_GET 0x03

start:
    LDI R16, UNUSED_ADDRESS_MAX
    LDP SP, R16
    XOR R16, R16

    LDI R1, 10
    LDI R2, 10
    LDI R3, 10
    LDI R4, 10
    LDI R5, COLOR_WHITE
    CALL DrawRect

done:
    IN R1, PORT_PIT_GET
    IN R2, PORT_KBD_IN
    JMP done


; IN:
;   R1: X
;   R2: Y
;   R3: Color
; OUT:
;   A pixel on the screen
DrawPixel:
    PUSH R4
    PUSH R5

    ; Y = 0 is bad for some reason
    INC R2

    ; Just in case we reach the end of VRAM
    PUSH R6

    LDI R4, VRAM_DWORD_START
    LDI R5, SCREEN_WIDTH
    ADD R4, R1
    MUL R2, R5
    ADD R4, R2
    
    LDP P1, R4
    STOD P1, R3

    POP R6
    POP R5
    POP R4
    RET

panic:
    JMP panic


; R1: X
; R2: Y
; R3: W (Width)
; R4: H (Height)
; R5: Color

DrawRect:
    PUSH R10
    PUSH R11
    PUSH R12
    PUSH R13
    PUSH R14
    PUSH R15
    PUSH R16
    CPY R16, R5     ; Color
    CPY R15, R4     ; Height
    CPY R13, R3     ; Width
    CPY R12, R2     ; Y
    CPY R11, R1     ; X
    CPY R14, R1     ; Store original X
    LDI R10, 0

DrawRectOuterLoop:
    CPY R9, R13      ; Copy Width to R9 (loop counter for X)

DrawRectInnerLoop:
    ; Draw pixel at (R11, R12) with color R16
    CPY R1, R11
    CPY R2, R12
    CPY R3, R16
    CALL DrawPixel

    INC R11          ; Move to next X position
    DEC R9           ; Decrement X loop counter
    CMP R9, R10
    BRE EndInnerLoop ; If X counter is zero, exit inner loop
    JMP DrawRectInnerLoop

EndInnerLoop:
    CPY R11, R14     ; Reset X to original X
    INC R12          ; Move to next Y position
    DEC R15          ; Decrement Height counter
    CMP R15, R10
    BRE EndOuterLoop ; If Height counter is zero, exit outer loop
    JMP DrawRectOuterLoop

EndOuterLoop:
    POP R16
    POP R15
    POP R14
    POP R13
    POP R12
    POP R11
    POP R10
    RET


