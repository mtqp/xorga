EJ = prision

all: $(EJ)

test/test_libre.in:
test/test_no_libre.in:
	tests/generador_tests.py
	mv test_libre.in tests/
	mv test_no_libre.in tests/

diffs:test/test_libre.in test/test_no_libre.in
	./$(EJ) < tests/test_libre.in | grep -c libre
	./$(EJ) < tests/test_no_libre.in | grep -c no

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
