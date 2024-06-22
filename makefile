all: compile run clean

compile:
	gcc src/main.c -o build/main -O3 -lSDL2 -w

assemble:
	python3 src/assembler.py

run:
	build/main bin/test.bin

clean: 
	rm -rf build/*