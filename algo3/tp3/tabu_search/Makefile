EJ = $(patsubst %.cpp,%,$(wildcard *.cpp))
FLAGS = -Wall
TEST_PATH = ../test/
PARAM_TEST = $(TEST_PATH)grafo_random_n_fijo.in

.PHONY: test_param

ejercicio: $(EJ)
test: $(patsubst $(TEST_PATH)%.in,%.out,$(wildcard $(TEST_PATH)*.in) )

test_param: $(EJ)
	./$(EJ) tamaño parametro 20 < $(PARAM_TEST) > param_20.out
	./$(EJ) tamaño parametro 40 < $(PARAM_TEST) > param_40.out
	./$(EJ) tamaño parametro 60 < $(PARAM_TEST) > param_60.out
	./$(EJ) tamaño parametro 80 < $(PARAM_TEST) > param_80.out
	./$(EJ) tamaño parametro 100 < $(PARAM_TEST) > param_100.out
	
test_param_time: $(EJ)
	./$(EJ) time parametro 20 < $(PARAM_TEST) > param_20.time
	./$(EJ) time parametro 40 < $(PARAM_TEST) > param_40.time
	./$(EJ) time parametro 60 < $(PARAM_TEST) > param_60.time
	./$(EJ) time parametro 80 < $(PARAM_TEST) > param_80.time
	./$(EJ) time parametro 100 < $(PARAM_TEST) > param_100.time


all: $(EJ) grafos

%:%.cpp
	g++ -o $@ $< $(FLAGS)

%.out: $(TEST_PATH)%.in
	./$(EJ) < $< > $@
	
%.eps: %.out
	@../graficar-grafos.py $(TEST_PATH)$(patsubst %.out,%.in,$<) $<
	@echo "Generando $@"
	dot -Tps $<.dot > $@
	@rm $<.dot

grafos: $(EJ) test $(patsubst $(TEST_PATH)%.in,%.eps,$(wildcard $(TEST_PATH)*.in))

clean:
	rm -f $(EJ)
	rm -f *.out
	rm -f *.eps

.PHONY: clean grafos
