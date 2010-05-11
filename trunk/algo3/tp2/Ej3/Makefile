EJ = prision

all: $(EJ)

diffs:
	./$(EJ) < tests/test_libre.in | grep -c libre
	./$(EJ) < tests/test_mejor_caso.in | grep -c no

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
