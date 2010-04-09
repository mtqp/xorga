FLAGS = -lm -g -Wall
OBJS = auxiliares.o case_generator.o 
 
main: main.cpp $(OBJS)
	g++ -o case_generator $(FLAGS) main.cpp $(OBJS)

clean:
	rm -f case_generator
	rm -f *o

