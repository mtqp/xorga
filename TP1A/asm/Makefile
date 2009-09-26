CC=gcc
CFLAGS=-c
OBJECTS = main.o filters.o filters_asm.o asmSobel.o

bordes: $(OBJECTS)
	$(CC) $(OBJECTS) -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c
	$(CC) $(CFLAGS) main.c `pkg-config --cflags opencv`

filters.o: filters.c filters.h
	$(CC) $(CFLAGS) filters.c

filters_asm.o: filters_asm.asm
	nasm -g -f elf filters_asm.asm
asmSobel.o: asmSobel.asm
	nasm -g -f elf asmSobel.asm

clean:
	rm -f $(OBJECTS)
