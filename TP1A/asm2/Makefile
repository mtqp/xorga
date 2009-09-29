CC=gcc
CFLAGS=-c
OBJECTS = main.o filters.o apply_mask.o asmSobel.o

bordes: $(OBJECTS)
	$(CC) $(OBJECTS) -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c
	$(CC) $(CFLAGS) main.c `pkg-config --cflags opencv`

filters.o: filters.c filters.h
	$(CC) $(CFLAGS) filters.c

%.o: %.asm
	nasm -g -f elf $<

clean:
	rm -f $(OBJECTS)
