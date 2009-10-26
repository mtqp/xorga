CC=gcc
CFLAGS=-c
OBJECTS = main_matrices.o apply_line.o 

mmain: $(OBJECTS)
	$(CC) $(OBJECTS) -o mmain `pkg-config --cflags --libs opencv`

main_matrices.o: main_matrices.c
	$(CC) $(CFLAGS) main_matrices.c `pkg-config --cflags opencv`

%.o: %.asm
	nasm -g -f elf $<

clean:
	rm -f $(OBJECTS)
