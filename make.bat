@echo off

rem Make sure to change the SDL2 include directories based on where you have it installed. Otherwise, this should work on all Windows machines.
gcc src/main.c -o bin/main.exe -I C:/clib/SDL2/include -L C:/clib/SDL2/lib/x64 -lSDL2main -lSDL2

python src/assembler.py src/game.asm bin/game.bin

cd bin
main.exe game.bin
del main.exe
del game.bin
cd..