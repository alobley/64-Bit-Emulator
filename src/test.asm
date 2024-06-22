; Origin
ORG 0

; Constants
%define VRAM_DWORD_START 124961600
%define VRAM_DWORD_END 125019200
%define INT_MAX 0xFFFFFFFF
%define PORT_KBD_IN 0x01

; Entry point
_start:
    LDI R1, INT_MAX
    LDI R2, VRAM_DWORD_START
    LDI R4, 97
    LDI R5, VRAM_DWORD_END

; The main loop
loop:
    GEP R6, P1                  ; Get the current value of P1 and store it in R6
    CMP R6, R5                  ; Compare the value of R6 (P1) to R5 (VRAM end)
    BRE done                    ; If they are equal, the program is finished.
    IN PORT_KBD_IN, R3          ; Get the current key pressed
    CMP R3, R4                  ; Compare it to 97 (A)
    BNE loop                    ; If it is not A, go bak to the loop

    ; If it is A, continute execution. This fills the whole screen with white.
    LDP P1, R2                  ; Put the value of R2 into P
    INC R2                      ; Increment R2
    STOD P1, R1                 ; Store the lower dword of R1 into the address pointed to by P1
    JMP loop                    ; Repeat the loop

; The program is finished, infinite loop
done:
    JMP done
