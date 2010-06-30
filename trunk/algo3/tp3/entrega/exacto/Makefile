EJ = $(patsubst %.cpp,%,$(wildcard *.cpp))
FLAGS = -Wall
TEST_PATH = ../test/

ejercicio: $(EJ)
test: $(patsubst $(TEST_PATH)%.in,%.out,$(wildcard $(TEST_PATH)*.in) )

all: $(EJ) grafos

%:%.cpp
	g++ -o $@ $< $(FLAGS)

%.out: $(TEST_PATH)%.in
	./$(EJ) < $< > $@
	
%.eps: %.out
	@../graficar-grafos.py $(TEST_PATH)$(patsubst %.out,%.in,$<) $<
	@echo "Generando $@"
	@dot -Tps $<.dot > $@
	@rm $<.dot

grafos: $(EJ) test $(patsubst $(TEST_PATH)%.in,%.eps,$(wildcard $(TEST_PATH)*.in))

clean:
	rm -f $(EJ)
	rm -f *.out
	rm -f *.eps

.PHONY: clean grafos
