EJ = secuencia_unimodal
CFLAGS = -Wall -O3
TESTS = $(patsubst tests/%.in,tests/%.out,$(wildcard tests/*.in))

all: $(EJ) 
tests: $(TESTS)

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

%.out: %.in
	./$(EJ) < $< > $@

clean:
	rm $(EJ)
