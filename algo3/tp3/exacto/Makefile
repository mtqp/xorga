#EJ = $(patsubst %.cpp,%,$(wildcard *.cpp))
EJ = exacto
FLAGS = -Wall
TEST_PATH = ../test/

ejercicio: $(EJ)
test: $(patsubst $(TEST_PATH)%.in,%.out,$(wildcard $(TEST_PATH)*.in) )

all: $(EJ) grafos

%:%.cpp
	g++ -o $@ $< $(FLAGS)

%.out: $(TEST_PATH)%.in
	@./$(EJ) < $< > $@
	
%.eps: $(TEST_PATH)%.in %.out
	@../graficar-grafos.py $^
	@echo "Generando $@"
	@dot -Tps $< > $@

#grafos: $(EJ) $(patsubst %.in,%.out.dot,$(wildcard *.in))
#	@make -f ../Makefile grafos
grafos: $(EJ) $(patsubst %.in,%.eps,$(wildcard $(TEST_PATH)/*.in))

clean:
	rm -f $(EJ)
	@make -f ../Makefile clean

.PHONY: clean grafos
