EJ = secuencia_unimodal
CFLAGS = -Wall -O3
TESTS = $(patsubst tests/%.in,$(wildcard tests/*.in))
TESTS_IN = $(wildcard tests/*.in)

all: $(EJ)

tests/%.in:
	tests/generador_tests.py
	mv test_creciente.in tests/
	mv test_decreciente.in tests/
	mv test_unimodal.in tests/

diffs: tests/test_creciente.in tests/test_decreciente.in tests/test_unimodal.in
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
