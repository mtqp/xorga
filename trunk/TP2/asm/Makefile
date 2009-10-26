CC=gcc
CFLAGS=-c
OBJECTS = main.o asmSobel.o asmPrewitt.o
#asmRoberts.o asmPrewitt.o

bordes: $(OBJECTS)
	$(CC) $(OBJECTS) -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c
	$(CC) $(CFLAGS) main.c `pkg-config --cflags opencv`

%.o: %.asm
	nasm -g -f elf $<

clean:
	rm -f $(OBJECTS)
