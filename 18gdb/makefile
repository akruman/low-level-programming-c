# install in root: sudo apt install kcachegrind
all:
	nasm -o 1.o -f elf64 1.asm
	ld -o 1 1.o

	gcc -ggdb -o 2 2.c

clean:
	rm -rf 1.o 1 2

debug:
	gdb ./1

debug1:
	gdb -x cmd.txt

debug2:
	gdb -x cmd2.txt

run:
	./1