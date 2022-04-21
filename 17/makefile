# install in root: sudo apt install kcachegrind
all:
	gcc -S -masm=intel -o 1.O0.asm -O0 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O1.asm -O1 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O2.asm -O2 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -S -masm=intel -o 1.O3.asm -O3 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -o 1 --std=c99 -Wall -Werror --pedantic 1.c
	gcc -o 2 --std=c99 -pthread -Wall -Werror --pedantic 2.c
	gcc -o 3 --std=c99 -pthread -Wall -Werror --pedantic 3.c
clean:
	rm -rf 1 1.asm 1.O* 2 3

run:
	valgrind -v  --tool=helgrind  ./2