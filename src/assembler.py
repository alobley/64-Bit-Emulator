import struct
import re

# All the instructions that can be assembled
instructions = { "NOP": 0b0000000000000000000000000000000000000000000000000000000000000000, "LDI": 0b0000000000000000000000000000000000000000000000000000000000000001,
                 "CPY": 0b0000000000000000000000000000000000000000000000000000000000000010, "LDP": 0b0000000000000000000000000000000000000000000000000000000000000011,
                 "GEP": 0b0000000000000000000000000000000000000000000000000000000000010110, "ADD": 0b0000000000000000000000000000000000000000000000000000000000000100,
                 "SUB": 0b0000000000000000000000000000000000000000000000000000000000000101, "MUL": 0b0000000000000000000000000000000000000000000000000000000000000110,
                 "DIV": 0b0000000000000000000000000000000000000000000000000000000000000111, "OR": 0b0000000000000000000000000000000000000000000000000000000000001000,
                 "XOR": 0b0000000000000000000000000000000000000000000000000000000000001001, "AND": 0b0000000000000000000000000000000000000000000000000000000000001010,
                 "NOT": 0b0000000000000000000000000000000000000000000000000000000000001011, "SHL": 0b0000000000000000000000000000000000000000000000000000000000001100,
                 "SHR": 0b0000000000000000000000000000000000000000000000000000000000001101, "JMP": 0b0000000000000000000000000000000000000000000000000000000000001110,
                 "BRE": 0b0000000000000000000000000000000000000000000000000000000000001111, "BNE": 0b0000000000000000000000000000000000000000000000000000000000010000,
                 "BRZ": 0b0000000000000000000000000000000000000000000000000000000000010001, "BNZ": 0b0000000000000000000000000000000000000000000000000000000000010010,
                 "LOAD": 0b0000000000000000000000000000000000000000000000000000000000010011, "STOR": 0b0000000000000000000000000000000000000000000000000000000000010100,
                 "CLF": 0b0000000000000000000000000000000000000000000000000000000000010101, "PUSH": 0b0000000000000000000000000000000000000000000000000000000000010111,
                 "POP": 0b0000000000000000000000000000000000000000000000000000000000011000, "CMP": 0b0000000000000000000000000000000000000000000000000000000000011001,
                 "GOTO": 0b0000000000000000000000000000000000000000000000000000000000011010, "STOD": 0b0000000000000000000000000000000000000000000000000000000000011011,
                 "INC": 0b0000000000000000000000000000000000000000000000000000000000011100, "DEC": 0b0000000000000000000000000000000000000000000000000000000000011101,
                 "IN": 0b0000000000000000000000000000000000000000000000000000000000011110, "OUT": 0b0000000000000000000000000000000000000000000000000000000000011111 }

# The numerical IDs of all the registers
registerIDs = { "R1": 0x00, "R2": 0x01, "R3": 0x02, "R4": 0x03, "R5": 0x04, "R6": 0x05, "R7": 0x06, "R8": 0x07, "R9": 0x08, "R10": 0x09, "R11": 0x0A,
                "R12": 0x0B, "R13": 0x0C, "R14": 0x0D, "R15": 0x0E, "R16": 0x0F, "P1": 0x10, "P2": 0x11, "SP": 0x12 }

# Define dictionaries that describe labels, macros, and constants in the assembly program, which can be used later.
labels = {}
macros = {}
constants = {}

memoryOffset = 0

# The origin point of the program (default 0)
org = 0


currentLine = 0         # The current line being assembled
assembledCode = []      # An array containing the raw binary that was assembled
inputString = ""        # The text of the file that is being assembled

# Allows the assembler to know how many instructions have been processed
offset = 0

# Returns true if a string is a hexadecimal number, false otherwise
def IsHex(s):
    if s.startswith('0X'):
        return True
    elif s.endswith('H'):
        return True
    else:
        return False

# Returns true if a string is a binary number, false otherwise
def IsBinary(s):
    if s.startswith('0B'):
        return True
    elif s.endswith('B'):
        return True
    else:
        return False

# Turns a binary number defined in a string into an integer
def BinToInt(s):
    if s.startswith('0B'):
        return int(s[2:], 2)
    elif s.endswith('B'):
        return int(s[:-1], 2)

# Turns a hexadecimal number defined in a string into an integer
def HexToInt(s):
    if s.startswith('0X'):
        return int(s[2:], 16)
    elif s.endswith('H'):
        return int(s[:-1], 16)

# Gets the origin point of the program if it is explicitly defined
def GetOrg(lines):
    global org
    for line in lines:
        line = line.upper()
        line = line.strip()
        if line.split(" ")[0] == "ORG":
            if line.split(" ")[1].isnumeric():
                org = int(line.split(" ")[1])
            elif line.split(" ")[1] in constants:
                org = constants[line.split(" ")[1]]
            elif IsHex(line.split(" ")[1]):
                org = HexToInt(line.split(" ")[1])
            elif IsBinary(line.split(" ")[1]):
                org = BinToInt(line.split(" ")[1])

# Gets the location and memory offset of all the labels in the program
def GetLabels(lines):
    global macros
    global constants
    global org
    offset = 0
    for line in lines:
        if ';' in line:
            line = line.split(";")[0]
            line = line.strip()
        line = line.upper()
        line = line.strip()
        if line and line[-1] == ":":
            labels[line[:-1]] = offset + org

        elif line and not line.split(" ")[0] in macros and not line.split()[1] in constants and not line.split(" ")[0] == "ORG":
            offset += 3
    offset = 0

# Gets all the macros in the program. Unfinished.
def GetMacros(lines):
    global macros
    inMacro = False
    macroName = ""
    macroInstructions = []
    for line in lines:
        if ';' in line:
            line = line.split(";")[0]
            line = line.strip()
        line = line.upper()
        line = line.strip()
        if line.split(" ")[0] == "%MACRO":
            macroName = line.split(" ")[1]
            inMacro = True
        if line and inMacro == True and line != "%ENDM":
            macroInstructions.append(line)
        if line == "%ENDM":
            inMacro = False
            macros[macroName] = macroInstructions

# Get the constants defined in the program, if any
def GetConstants(lines):
    global constants
    constVal = 0
    for line in lines:
        if ';' in line:
            line = line.split(";")[0]
            line = line.strip()

        line = line.upper()
        line = line.strip()
        if line.split(" ")[0] == "%DEFINE":
            if line.split(" ")[2].isnumeric():
                constVal = int(line.split(" ")[2])
                print(str(constVal))
            elif IsHex(line.split(" ")[2]):
                constVal = HexToInt(line.split(" ")[2])
                print(str(constVal))
            elif IsBinary(line.split(" ")[2]):
                constVal = BinToInt(line.split(" ")[2])
                print(str(constVal))

            constants[line.split(" ")[1]] = constVal

# Assembles parsed instructions into their binary output
def AssembleInstruction(opcode, operand1, operand2):
    global assembledCode

    return struct.pack('QQQ', opcode, operand1, operand2)


# Parses instructions by splitting the file up line by line, separating the opcode and operands, and getting their numerical values
def ParseInstruction(line):
    global labels
    global macros
    global constants
    global currentLine
    global memoryOffset

    line = line.upper()
    line = line.strip()

    opcode = 0
    operand1 = 0
    operand2 = 0

    currentLine += 1

    # Add macro expansion later

    if ';' in line:
        line = line.split(";")[0]
        line = line.strip()

    if line and line[-1] == ":":
        return
    if line and line.split(" ")[0] == "ORG":
        return
    if line.strip().split(" ")[0] == "%DEFINE":
        return
    if line and line.split(" ")[0] in instructions:
        opcode = instructions[line.split(" ")[0]]

        if len(line.split(" ")) > 2:
            if line.split(" ")[1][:-1].isnumeric():
                operand1 = int(line.split(" ")[1][:-1])
            elif line.split(" ")[1][:-1] in constants:
                operand1 = constants[line.split(" ")[1][:-1]]
            elif IsHex(line.split(" ")[1][:-1]):
                operand1 = HexToInt(line.split(" ")[1][:-1])
            elif IsBinary(line.split(" ")[1][:-1]):
                operand1 = BinToInt(line.split(" ")[1][:-1])
            elif line.split(" ")[1][:-1] in registerIDs:
                operand1 = registerIDs[line.split(" ")[1][:-1]]
            elif line.split(" ")[1][:-1] in labels:
                operand1 = labels[line.split(" ")[1][:-1]]


            if line.split(" ")[2].isnumeric():
                operand2 = int(line.split(" ")[2])
            elif line.split(" ")[2] in constants:
                operand2 = constants[line.split(" ")[2]]
            elif IsHex(line.split(" ")[2]):
                operand2 = HexToInt(line.split(" ")[2])
            elif IsBinary(line.split(" ")[2]):
                operand2 = BinToInt(line.split(" ")[2])
            elif line.split(" ")[2] in registerIDs:
                operand2 = registerIDs[line.split(" ")[2]]
            elif line.split(" ")[2] in labels:
                operand2 = labels[line.split(" ")[2]]


        elif len(line.split(" ")) > 1:
            if line.split(" ")[1].isnumeric():
                operand1 = int(line.split(" ")[1])
            elif line.split(" ")[1] in constants:
                operand1 = constants[line.split(" ")[1]]
            elif IsHex(line.split(" ")[1]):
                operand1 = HexToInt(line.split(" ")[1])
            elif IsBinary(line.split(" ")[1]):
                operand1 = BinToInt(line.split(" ")[1])
            elif line.split(" ")[1] in registerIDs:
                operand1 = registerIDs[line.split(" ")[1]]
            elif line.split(" ")[1] in labels:
                operand1 = labels[line.split(" ")[1]]

            operand2 = 0
        else:
            operand1 = 0
            operand2 = 0

    if line:
        assembledCode.append(AssembleInstruction(opcode, operand1, operand2))
        memoryOffset += 3


# The main function
def main():
    inFileName = input("Enter the name of your asm file: ")
    outFileName = input("Enter the name of your assembled code: ")

    with open(inFileName, 'r') as assemblyFile:
        inputString = assemblyFile.read()

    lines = inputString.split('\n')
    GetMacros(lines)
    GetConstants(lines)
    GetOrg(lines)
    GetLabels(lines)

    for line in lines:
        ParseInstruction(line)

    with open(outFileName, 'wb') as binary_file:
        for i in range(len(assembledCode)):
            binary_file.write(assembledCode[i])

main()