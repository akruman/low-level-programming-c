.PHONY: all clean

SRC:=$(wildcard *.asm)
OBJ:=$(patsubst %.asm,%.o, $(SRC))
EXE:=$(patsubst %.asm,%, $(SRC))

all: $(EXE)

clean: 
	rm -rf $(EXE) $(OBJ)

%.o : %.asm
	nasm -felf64 $< -o $@

% : %.o
	ld -o $@ $<
	chmod u+x $@