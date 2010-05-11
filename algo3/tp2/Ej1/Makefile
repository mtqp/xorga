EJ = secuencia_unimodal
CFLAGS = -Wall -O3
TESTS = $(patsubst tests/%.in,tests/%.out,$(wildcard tests/*.in))
TESTS_IN = $(wildcard tests/*.in)

all: $(EJ) 
tests: tests/secuencia_unimodal.out $(TESTS)

diffs:
	./$(EJ) < tests/test_creciente.in | grep -c 0
	./$(EJ) < tests/test_decreciente.in | grep -c 0
	./$(EJ) < tests/test_unimodal.in | grep -c 0

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

%.out: %.in
	./$(EJ) < $< > $@

tests/secuencia_unimodal.out:
	tests/generador_tests.py

clean:
	rm $(EJ)
