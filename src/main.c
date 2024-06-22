/* Program: 64-bit CPU emulator
 * Developer: xWatexx
 * Version: Alpha 2
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "lib/types.h"
#include "lib/instructions.h"
#include "lib/ports.h"
#include <SDL2/SDL.h>

uint64* memory;

uint64 registers[16];
uint64 Ptr1 = 0;
uint64 Ptr2 = 0;

uint64 IR = 0;
uint64 DR1 = 0;
uint64 DR2 = 0;

uint64 PC = 0;
uint64 SPtr = 0;

// Equal, Zero, Sign, Overflow
bool FLAGS[4];

#pragma region timer
bool timerEvent = false;
SDL_TimerID timer;

uint64 PITCallback(uint32 interval, void* param){
    // Timer event occurred
    *((bool*)param) = true;

    // Return the interval to continue the timer
    return interval;
}

uint64 ReadPIT(){
    timerEvent = false;
    return true;
}

void WritePIT(uint64 value){
    timer = SDL_AddTimer(value, PITCallback, &timerEvent);
}

#pragma endregion

#pragma region graphics
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void InitSDL(){
    // Actual resolution is 320x180, but it is upscaled tp 720p for better viewing.
    window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    timer = SDL_AddTimer(1, PITCallback, &timerEvent);
}

void DrawPixel(uint64 x, uint64 y, uint32 color){
    uint8 red = 0;
    uint8 green = 0;
    uint8 blue = 0;
    uint8 alpha = 0;

    if(color != 0){
        red = (uint8)color;
        green = (uint8)(color >> 8);
        blue = (uint8)(color >> 16);
        alpha = (uint8)(color >> 24);
    }

    SDL_Rect r = {x, y, 4, 4};

    SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

    SDL_RenderFillRect(renderer, &r);
}
uint32* startPtr;
void UpdateScreen(){
    for(int i = 0; i < 180; i++){
        for(int j = 0; j < 320; j++){
            uint32* intptr = startPtr + (i * 320) + j;
            DrawPixel(j * 4, i * 4, *intptr);
        }
    }
    SDL_RenderPresent(renderer);
}

void CloseSDL(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#pragma endregion

#pragma region InstructionProcessing
bool jmpinstruction = false;

uint64 keyboardInput = 0;

uint64 ProcessInput(uint64 port){
    if(port = PORT_KBD_READ){
        return (uint64)keyboardInput;
    }
    printf("No valid port!\n");
    return 0;
}

void ProcessOutput(uint64 port, uint64 reg){
    return;
}

// This will process instructions and operands given
void ProcessInstruction(uint64 instruction, uint64 operand1, uint64 operand2){
    IR = instruction;
    DR1 = operand1;
    DR2 = operand2;

    switch(IR){
        case NOP:
            DR1 = 0;
            DR2 = 0;
            return;
        case LDI:
            if(DR1 <= R16){
                registers[DR1] = DR2;
            }else{
                // Add error stuff later
                printf("Invalid register in LDI instruction!\n");
            }
            return;
        case CPY:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR2];
            }else{
                printf("Invalid register(s) in CPY instruction!\n");
            }
            return;
        case LDP:
            if(DR1 == P1 || DR1 == P2 && DR2 <= R16){
                if(DR1 == P1){
                    Ptr1 = registers[DR2];
                }else if(DR1 == P2){
                    Ptr2 = registers[DR2];
                }else{
                    SPtr = registers[DR2];
                }
            }else{
                printf("Invalid register(s) in LDP instruction!\n");
            }
            return;
        case GEP:
            // (gpreg, ptr)
            if(DR2 == P1 || DR2 == P2 && DR1 <= R16){
                if(DR2 == P1){
                    registers[DR1] = Ptr1;
                }else{
                    registers[DR1] = Ptr2;
                }
            }else{
                printf("Invalid register(s) in GEP instruction!\n");
            }
            return;
        case ADD:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] += registers[DR2];
                if(registers[DR1] + registers[DR2] > ULONG_MAX){
                    FLAGS[3] = true;
                }
            }else{
                printf("Invalid register(s) in ADD instruction!\n");
            }
            return;
        case SUB:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] -= registers[DR2];
                if(registers[DR1] - registers[DR2] < 0){
                    FLAGS[2] = true;
                }
            }else{
                printf("Invalid register(s) in SUB instruction!\n");
            }
            return;
        case MUL:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] *= registers[DR2];
                if(registers[DR1] * registers[DR2] > ULONG_MAX){
                    FLAGS[3] = true;
                }
            }else{
                printf("Invalid register(s) in MUL instruction!\n");
            }
            return;
        case DIV:
            // Dividend, divisor
            // Remainer will always be in Register 16
            if(DR1 <= R16 && DR2 <= R16){
                registers[R16] = registers[DR1] % registers[DR2];
                registers[DR1] /= registers[DR2];
            }else{
                printf("Invalid register(s) in DIV instruction!\n");
            }
            return;
        case OR:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR1] | registers[DR2];
            }else{
                printf("Invalid register(s) in OR instruction!\n");
            }
            return;
        case XOR:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR1] ^ registers[DR2];
            }else{
                printf("Invalid register(s) in XOR instruction!\n");
            }
            return;
        case AND:
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR1] & registers[DR2];
            }else{
                printf("Invalid register(s) in AND instruction!\n");
            }
            return;
        case NOT:
            // One operand
            if(DR1 <= R16){
                registers[DR1] = !registers[DR1];
            }else{
                printf("Invalid register in NOT instruction!\n");
            }
            return;
        case SHL:
            // One operand
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR1] << 1;
            }else{
                printf("Invalid register in SHL instruction!\n");
            }
            return;
        case SHR:
            // One operand
            if(DR1 <= R16 && DR2 <= R16){
                registers[DR1] = registers[DR1] >> 1;
            }else{
                printf("Invalid register(s) in SHR instruction!\n");
            }
            return;
        case JMP:
            // One operand, immediate
            PC = DR1;
            jmpinstruction = true;
            return;
        case BRE:
            if(FLAGS[0] == true){
                PC = DR1;
                jmpinstruction = true;
            }
            return;
        case BNE:
            if(FLAGS[0] == false){
                PC = DR1;
                jmpinstruction = true;
            }
            return;
        case BRZ:
            if(FLAGS[1] == true){
                PC = DR1;
                jmpinstruction = true;
            }
            return;
        case BNZ:
            if(FLAGS[1] == false){
                PC = DR1;
                jmpinstruction = true;
            }
            return;
        case LOAD:
            if((DR1 == P1 || DR1 == P2) && DR2 <= R16){
                if(DR1 == P1){
                    registers[DR2] = ((uint64*)memory)[P1];
                }else{
                    registers[DR2] = ((uint64*)memory)[P2];
                }
            }else{
                printf("Invalid register(s) in LOAD instruction!\n");
            }
            return;
        case STOR:
            if((DR1 == P1 || DR1 == P2) && DR2 <= R16){
                if(DR1 == P1){
                    ((uint64*)memory)[Ptr1] = registers[DR2];
                }else if(DR1 == P2){
                    ((uint64*)memory)[Ptr2] = registers[DR2];
                }else{
                    printf("Invalid register(s) in STOR instruction!\n");
                }
            }
            return;
        case CLF:
            for(int i = 0; i < 4; i++){
                FLAGS[i] = false;
            }
            return;
        case PUSH:
            // Registers only
            if(DR1 <= R16){
                memory[SPtr] = registers[DR1];
                SPtr++;
            }else if(DR1 == $){
                // Allows for call (push $)
                memory[SPtr] = PC;
                SPtr++;
            }else{
                printf("Invalid register in PUSH instruction!\n");
            }
            return;
        case POP:
            // Registers only
            if(DR1 <= R16){
                registers[DR1] = memory[SPtr];
                SPtr--;
            }else if(DR1 == $){
                // Allows for ret (pop $)
                PC = memory[SPtr];
                SPtr--;
            }else{
                printf("Invalid register in POP instruction!\n");
            }
            return;
        case CMP:
            if(DR1 <= R16 && DR2 <= R16){
                if(registers[DR1] == registers[DR2]){
                    FLAGS[0] = true;
                }else{
                    FLAGS[0] = false;
                }
                if(registers[DR1] && registers[DR2] == 0){
                    FLAGS[1] = true;
                }else{
                    FLAGS[1] = false;
                }
            }else{
                printf("Invalid register(s) in CMP instruction!\n");
            }
            return;
        case GOTO:
            if(DR1 == P1){
                ProcessInstruction(JMP, P1, 0);
            }else if(DR1 == P2){
                ProcessInstruction(JMP, P2, 0);
            }else{
                printf("Invalid register(s) in GOTO instruction!\n");
            }
            return;
        case STOD:
            // Only the lower 32 bits of the register you use will be sent
            if(DR1 == P1){
                uint32* intptr = (uint32*)memory + Ptr1;
                *intptr = (uint32)(registers[DR2]);
            }else if(DR1 == P2){
                uint32* intptr = (uint32*)memory + Ptr1;
                *intptr = (uint32)(registers[DR2]);
            }else{
                printf("Invalid register(s) in STOD instruction!\n");
            }
            return;
        case INC:
            if(DR1 <= R16){
                registers[DR1]++;
            }else{
                printf("Invalid register in INC instruction!\n");
            }
            return;
        case DEC:
            if(DR1 <= R16){
                registers[DR1]--;
            }else{
                printf("Invalid register(s) in DEC instruction!\n");
            }
            return;
        case IN:
            // port, register
            if(DR2 <= R16){
                registers[DR2] = ProcessInput(DR1);
            }else{
                printf("Invalid register in IN instruction\n");
            }
            return;
        case OUT:
            if(DR2 <= R16){
                ProcessOutput(DR1, registers[DR2]);
            }else{
                printf("Invalid register in OUT instruction\n");
            }
            return;
    }
    printf("Invalid instruction at address %lli!\n", PC);
}  
#pragma endregion

void PrintDbg(uint64* memory){
    for(int i = 0; i < 16; i++){
        printf("R%d: %lli\n", i+1, registers[i]);
    }
    printf("P1: %lli\n", Ptr1);
    printf("P2: %lli\n", Ptr2);
}

void ExecuteProgram(){
    SDL_Event e;
    PC = 0;
    uint64 instructionsExecuted = 0;
    while(true){
        ProcessInstruction(memory[PC], memory[PC+1], memory[PC+2]);
        if(jmpinstruction == false){
            PC += 3;
        }
        jmpinstruction = false;
        instructionsExecuted++;

        // Uhhh... if it works it works? This is actually double what is needed to account for future overhead. For some reason, this
        // if statement drastically improves performance and does not provide any noticeable delay whatsoever.
        if (instructionsExecuted % 163840 == 0) {
            UpdateScreen();
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT){
                    return;
                }
                if(e.type == SDL_KEYDOWN){
                    keyboardInput = e.key.keysym.sym;
                }
            }
        }
    }
}

void LoadProgram(uint64* ROM, uint64 length){
    for(uint64 i = 0; i < length; i++){
        memory[i] = ROM[i];
    }
}


// TODO:
// 1. Add I/O instruction (done)
// 2. Add a PIT (done)
// 3. (optional) at some point, add instructions for moving different data sizes to and from memory, rather than just dwords and qwords.
// 4. Create an assembler (most likely in Python)
// 5. Create a simple game to confirm the computer is good to use
// 6. Implement call/ret either as an assembler macro or as specific instructions
// 7. Add an FPU (maybe)
// 8. (Potentially) add interrupts (may be a bad idea)
// 9. Create virtual graphics hardware instead of a framebuffer, and hopefully go for HD with text mode support. Alternatively, I could try to add PCI support
//    for using something like VMware SVGA or other virtual VGA hardware.
// 10. Add a virtual disk connector for storing programs on a virtual hard disk
// 11. Create a BIOS
// 12. If interrupts were created, create a PIC
// 13. Create a simple CLI OS like DOS
// 14. (optional) Create a custom local machine only update of Clang or GCC that can compile C code to this architecture
// 15. Name the architecture of the CPU and the GPU

#define DWORD_BUFFER_START 124961600

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("No input file was specified!\n");
        return EXIT_FAILURE;
    }

    uint64 fileSize = 0;
    FILE *file = fopen(argv[1], "rb");
    if(file == NULL){
        fprintf(stderr, "Error opening file. Does it exist?\n");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    uint64* ROM = (uint64 *)malloc(fileSize);

    fread(ROM, 1, fileSize, file);

    memory = (uint64 *)malloc(1000000000);
    if(memory == NULL){
        fprintf(stderr, "Failed to allocate enough memory for system RAM!\n");
        exit(EXIT_FAILURE);
    }
    startPtr = (uint32*)memory + 124961600;

    InitSDL();

    uint64 romLen = fileSize / sizeof(uint64);

    LoadProgram(ROM, romLen);

    ExecuteProgram();

    PrintDbg(memory);

    free(memory);
    CloseSDL();
    return 0;
}