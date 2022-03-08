all:
	nasm -f elf64 -o lib.o lib.asm
	nasm -f elf64 -o dict.o dict.asm
	nasm -f elf64 -o main.o main.asm
	ld -o main *.o
	chmod u+x main
clean:
	rm -rf *.o main