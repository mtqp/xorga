CC=gcc
CFLAGS=-c
OBJECTS = main.o asmSobel.o asmPrewitt.o asmRoberts.o asmFreiChen.o
NOSSEOBJS = nosse/asmSobel.o nosse/asmPrewitt.o nosse/asmRoberts.o nosse/apply_mask.o

bordes: $(OBJECTS) $(NOSSEOBJS)
	$(CC) $(OBJECTS) $(NOSSEOBJS) -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c
	$(CC) $(CFLAGS) main.c `pkg-config --cflags opencv`

%.o: %.asm
	nasm -g -f elf $<

clean:
	rm -f $(OBJECTS) $(NOSSEOBJS)
