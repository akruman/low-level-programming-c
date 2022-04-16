# install in root: sudo apt install kcachegrind
all:
	gcc -o 1a --std=c11 -Wall -Werror --pedantic 1.c
	gcc -o 1b -DPREFETCH --std=c11 -Wall -Werror --pedantic 1.c
	nasm -felf64 simd_asm.asm -o simd_asm.o
	gcc -o simd_main --std=c11 -Wall -Werror --pedantic simd_main.c simd_asm.o
clean:
	rm -rf 1a 1b 1.bin 1.txt simd_main simd_asm.o

run:
	/usr/bin/time -v ./1a
	/usr/bin/time -v ./1b

	valgrind --tool=cachegrind ./1a
	valgrind --tool=cachegrind ./1b