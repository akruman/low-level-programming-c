.PHONY: all clean

all: asd
clean:
	rm -rf asd asd.o asd.x

asd: asd.o asd.x
	@echo "$@ dependes on (1st): $<  (all): $^ (new):$? "
	cp asd.o asd

asd.x:
	touch asd.x

# $* matches % value
%.o : %.c
	echo target: $@ 
	echo stem: $*
	
	cp $*.c $*.o
