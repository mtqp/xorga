OBJECTS = Grafo.o main.o
FLAGS = -Wall

main: $(OBJECTS)
	g++ -o $@ $^ $(FLAGS)

main.o: main.cpp
	g++ -c $< $(FLAGS)

%.o: %.cpp %.h
	g++ -c $< $(FLAGS)

clean:
	rm $(OBJECTS) main
