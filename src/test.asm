org 0

%define VRAM_DWORD_START 124961600
%define INT_MAX 0xFFFFFFFF

_start:
    ldi R1, INT_MAX
    ldi R2, VRAM_DWORD_START

loop:
    ldp P1, R2
    inc R2
    stod P1, R1
    jmp loop
