#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define R16 0x0FULL

// The pointer registers
#define P1 0x10ULL
#define P2 0x11ULL
#define SP 0x12ULL

#define NOP 0x00ULL

// Load an immediate value into a general-purpose register
#define LDI 0x01ULL

// Copy the value of one general-purpose register to another
#define CPY 0x02ULL

// Load from a general-purpose register into a P register
#define LDP 0x03ULL

// Load the value of P into a general-purpose register to get the address it points to
#define GEP 0x04ULL

// ALU Operations (registers ONLY, no immediate values). First operand register is always where the output value will be

// Add two registers
#define ADD 0x05ULL

// Subtract two registers.
#define SUB 0x06ULL

// Multiply two registers
#define MUL 0x07ULL

// Divide. Modulus is stored in R16. May change that to a specific modulus operation at some point.
#define DIV 0x08ULL

// Bitwise OR operation on two registers
#define OR  0x09ULL

// Bitwise XOR operation on two registers
#define XOR 0x0AULL

// Bitwise AND operation on two registers
#define AND 0x0BULL

// Bitwise NOT operation on a single register
#define NOT 0x0CULL

// Bit shift left a register (essentially multiplying it by two)
#define SHL 0x0DULL

// Bit shift right a register (essentially dividing it by two)
#define SHR 0x0EULL

// Increment a general-purpose register
#define INC 0x0FULL

#define DEC 0x10ULL

// Branching (BRO, BGT, BLT and others may be added)

// Jump to an immediate memory address
#define JMP 0x11ULL

// Branch to an immediate memory address if the equal flag is 1
#define BRE 0x12ULL

// Branch to an immediate memory address if the equal flag is 0
#define BNE 0x13ULL

// Branch to an immediate memory address if the zero flag is 1
#define BRZ 0x14ULL

// Branch to an immediate memory address if the zero flag is 0
#define BNZ 0x15ULL

// JMP but with a pointer regitster instead of an immediate address
#define GOTO 0x16ULL

#define CALL 0x17ULL

#define RET 0x18ULL

// Compare. Sets flags for jumping
#define CMP 0x19ULL

// Memory pointer operations (register to P pointer)

// Load from the pointer at a P register into a general-purpose register (PR, GPR)
#define LOAD 0x1AULL

// Store the value in a general-purpose register to the pointer at a P register (PR, GPR)
#define STOR 0x1BULL

// Push a register onto the stack
#define PUSH 0x1CULL

// Pop from the stack and into a given register
#define POP  0x1DULL

// Store DWORD. Note that the address given must be twice that of the STOR instruction for the same physical address. May change later.
#define STOD 0x1EULL

// Clear Flags
#define CLF 0x1FULL

// IN/OUT/INT to be added soon

// Bring data from a port into a register
#define IN 0x20ULL

// Send data from a register to a port
#define OUT 0x21ULL

// CALL/RET are next. HLT may be good to add as well.


#endif