#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// The general-purpose registers
#define R1 0x00ULL
#define R2 0x01ULL
#define R3 0x02ULL
#define R4 0x03ULL
#define R5 0x04ULL
#define R6 0x05ULL
#define R7 0x06ULL
#define R8 0x07ULL
#define R9 0x08ULL
#define R10 0x09ULL
#define R11 0x0AULL
#define R12 0x0BULL
#define R13 0x0CULL
#define R14 0x0DULL
#define R15 0x0EULL
#define R16 0x0FULL

// The pointer registers
#define P1 0x10ULL
#define P2 0x11ULL
#define SP 0x12ULL

// Technically a register identifier, but it's the current memory address. Recommend only using for returning to a memory address after jumping.
#define $ 0xFFULL

#define NOP 0b0000000000000000000000000000000000000000000000000000000000000000ULL

// Load an immediate value into a general-purpose register
#define LDI 0b0000000000000000000000000000000000000000000000000000000000000001ULL

// Copy the value of one general-purpose register to another
#define CPY 0b0000000000000000000000000000000000000000000000000000000000000010ULL

// Load from a general-purpose register into a P register
#define LDP 0b0000000000000000000000000000000000000000000000000000000000000011ULL

// Load the value of P into a general-purpose register to get the address it points to
#define GEP 0b0000000000000000000000000000000000000000000000000000000000010110ULL

// ALU Operations (registers ONLY, no immediate values). First operand register is always where the output value will be

// Add two registers
#define ADD 0b0000000000000000000000000000000000000000000000000000000000000100ULL

// Subtract two registers.
#define SUB 0b0000000000000000000000000000000000000000000000000000000000000101ULL

// Multiply two registers
#define MUL 0b0000000000000000000000000000000000000000000000000000000000000110ULL

// Divide. Modulus is stored in R16. May change that to a specific modulus operation at some point.
#define DIV 0b0000000000000000000000000000000000000000000000000000000000000111ULL

// Bitwise OR operation on two registers
#define OR  0b0000000000000000000000000000000000000000000000000000000000001000ULL

// Bitwise XOR operation on two registers
#define XOR 0b0000000000000000000000000000000000000000000000000000000000001001ULL

// Bitwise AND operation on two registers
#define AND 0b0000000000000000000000000000000000000000000000000000000000001010ULL

// Bitwise NOT operation on a single register
#define NOT 0b0000000000000000000000000000000000000000000000000000000000001011ULL

// Bit shift left a register (essentially multiplying it by two)
#define SHL 0b0000000000000000000000000000000000000000000000000000000000001100ULL

// Bit shift right a register (essentially dividing it by two)
#define SHR 0b0000000000000000000000000000000000000000000000000000000000001101ULL

// Branching (BRO, BGT, BLT and others may be added)

// Jump to an immediate memory address
#define JMP 0b0000000000000000000000000000000000000000000000000000000000001110ULL

// Branch to an immediate memory address if the equal flag is 1
#define BRE 0b0000000000000000000000000000000000000000000000000000000000001111ULL

// Branch to an immediate memory address if the equal flag is 0
#define BNE 0b0000000000000000000000000000000000000000000000000000000000010000ULL

// Branch to an immediate memory address if the zero flag is 1
#define BRZ 0b0000000000000000000000000000000000000000000000000000000000010001ULL

// Branch to an immediate memory address if the zero flag is 0
#define BNZ 0b0000000000000000000000000000000000000000000000000000000000010010ULL

// Memory pointer operations (register to P pointer)

// Load from the pointer at a P register into a general-purpose register (PR, GPR)
#define LOAD 0b0000000000000000000000000000000000000000000000000000000000010011ULL

// Store the value in a general-purpose register to the pointer at a P register (PR, GPR)
#define STOR 0b0000000000000000000000000000000000000000000000000000000000010100ULL

// Clear Flags
#define CLF 0b0000000000000000000000000000000000000000000000000000000000010101ULL

// Push a register onto the stack
#define PUSH 0b0000000000000000000000000000000000000000000000000000000000010111ULL

// Pop from the stack and into a given register
#define POP  0b0000000000000000000000000000000000000000000000000000000000011000ULL

// Compare. Sets flags for jumping
#define CMP 0b0000000000000000000000000000000000000000000000000000000000011001ULL

// JMP but with a pointer regitster instead of an immediate address
#define GOTO 0b0000000000000000000000000000000000000000000000000000000000011010ULL

// Store DWORD. Note that the address given must be twice that of the STOR instruction for the same physical address. May change later.
#define STOD 0b0000000000000000000000000000000000000000000000000000000000011011ULL

// Increment a general-purpose register
#define INC 0b0000000000000000000000000000000000000000000000000000000000011100ULL
#define DEC 0b0000000000000000000000000000000000000000000000000000000000011101ULL

// IN/OUT/INT to be added soon

// Bring data from a port into a register
#define IN 0b0000000000000000000000000000000000000000000000000000000000011110ULL

// Send data from a register to a port
#define OUT 0b0000000000000000000000000000000000000000000000000000000000011111ULL


#endif