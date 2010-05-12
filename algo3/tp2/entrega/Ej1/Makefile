EJ = secuencia_unimodal
CFLAGS = -Wall -O3
TESTS = $(patsubst pruebas/%.in,$(wildcard pruebas/*.in))
TESTS_IN = $(wildcard tests/*.in)

all: $(EJ)

pruebas/%.in:
	pruebas/generador_tests.py
	mv test_creciente.in pruebas/
	mv test_decreciente.in pruebas/
	mv test_unimodal.in pruebas/

diffs: pruebas/test_creciente.in pruebas/test_decreciente.in pruebas/test_unimodal.in
	./$(EJ) < pruebas/test_creciente.in | grep -c 0
	./$(EJ) < pruebas/test_decreciente.in | grep -c 0
	./$(EJ) < pruebas/test_unimodal.in | grep -c 0

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

%.out: %.in
	./$(EJ) < $< > $@

pruebas/secuencia_unimodal.out:
	pruebas/generador_tests.py

clean:
	rm $(EJ)
