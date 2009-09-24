OBJECTS = main.o 

bordes: $(OBJECTS)
	gcc  $(OBJECTS) -g -o bordes `pkg-config --cflags --libs opencv`

main.o: main.c filters.o
	gcc -g -c main.c filters.o `pkg-config --cflags opencv`

filters.o: filters.c filters.h
	gcc -g -c filters.c -o filters.o

clean:
	rm -f $(OBJECTS)
