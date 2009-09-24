OBJECTS = main.o filters.o

bordes: $(OBJECTS)
	gcc  $(OBJECTS) -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c
	gcc -c main.c `pkg-config --cflags opencv`

filters.o: filters.c filters.h
	gcc -c filters.c -o filters.o

clean:
	rm -f $(OBJECTS)
