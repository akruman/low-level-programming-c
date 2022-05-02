# install in root: sudo apt install kcachegrind
.PHONY: all m5 clean rum
all: m5
	gcc -S -masm=intel -o 1.O0.asm -O0 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O1.asm -O1 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O2.asm -O2 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O3.asm -O3 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -o 1 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -o 2 --std=c99 -pthread -Wall -Werror --pedantic 2.c
	gcc -o 3 --std=c99 -pthread -Wall -Werror --pedantic 3.c
	gcc -g2 -o 4 --std=c99 -pthread -Wall -Werror --pedantic 4.c

m5:
	gcc -O2 -o 5 --std=c11 -pthread -Wall -Werror --pedantic 5.c
	objdump -S 5 --disassemble=main 

m6:
	gcc -O2 -o 6 --std=c11 -pthread -Wall -Werror --pedantic 6.c
	objdump -S 6
	diff -y <(objdump -S 6 --disassemble=f2__memory_order_consume) <(objdump -S 6 --disassemble=f2__memory_order_acquire)
	
clean:
	rm -rf 1 1.asm 1.O* 2 3 4 5 6

run:
	valgrind -v  --tool=helgrind  ./2