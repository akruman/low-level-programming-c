.PHONY: all clean run
all:
	nasm -f elf64 -o libso_main.o libso_main.asm
	nasm -f elf64 -o libso.o libso.asm
	ld -shared -o libso.so libso.o --dynamic-linker=/lib64/ld-linux-x86-64.so.2
	ld -o m libso_main.o -d libso.so
	chmod +x m

clean:
	rm -rf *.o *.so m

run: # not works yet
	LD_LIBRARY_PATH=. ./m