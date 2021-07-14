all: program
using_bisection: program_bisection

program: main.o func.o
	gcc -m32 func.o main.o -lm -o program

program_bisection: main.o_bisection func.o
	gcc -m32 func.o main.o -lm -o program

main.o: main.c
	gcc -m32 -c main.c -o main.o

main.o_bisection: main.c
	gcc -m32 -c main.c -DBISECTION -o main.o

func.o: func.asm
	nasm -f elf32 -o func.o func.asm

clean:
	rm -rf *.o program

