ASMFILE=game

all: assemble compile run clean

compile:
	gcc src/main.c -o build/main -O3 -lSDL2 -w

assemble:
	python3 src/assembler.py src/$(ASMFILE).asm bin/$(ASMFILE).bin

run:
	build/main bin/$(ASMFILE).bin

clean: 
	rm -rf build/*
	rm -rf bin/*