EJ = $(patsubst %.cpp,%,$(wildcard *.cpp))
FLAGS = -Wall
TEST_PATH = ../test/
PARAM_TEST = $(TEST_PATH)/inpute.in

.PHONY: test_param

ejercicio: $(EJ)
test: $(patsubst $(TEST_PATH)%.in,%.out,$(wildcard $(TEST_PATH)*.in) )

test_param: $(EJ)
	./$(EJ) tamaño parametro 5 < $(PARAM_TEST) > param_5.out
	./$(EJ) tamaño parametro 10 < $(PARAM_TEST) > param_10.out
	./$(EJ) tamaño parametro 15 < $(PARAM_TEST) > param_15.out
	./$(EJ) tamaño parametro 20 < $(PARAM_TEST) > param_20.out
	./$(EJ) tamaño parametro 25 < $(PARAM_TEST) > param_25.out
	./$(EJ) tamaño parametro 30 < $(PARAM_TEST) > param_30.out
	./$(EJ) tamaño parametro 35 < $(PARAM_TEST) > param_35.out
	./$(EJ) tamaño parametro 40 < $(PARAM_TEST) > param_40.out
	./$(EJ) tamaño parametro 45 < $(PARAM_TEST) > param_45.out
	./$(EJ) tamaño parametro 50 < $(PARAM_TEST) > param_50.out
	./$(EJ) tamaño parametro 55 < $(PARAM_TEST) > param_55.out
	./$(EJ) tamaño parametro 60 < $(PARAM_TEST) > param_60.out
	./$(EJ) tamaño parametro 65 < $(PARAM_TEST) > param_65.out
	./$(EJ) tamaño parametro 70 < $(PARAM_TEST) > param_70.out
	./$(EJ) tamaño parametro 75 < $(PARAM_TEST) > param_75.out
	./$(EJ) tamaño parametro 80 < $(PARAM_TEST) > param_80.out
	./$(EJ) tamaño parametro 85 < $(PARAM_TEST) > param_85.out
	./$(EJ) tamaño parametro 90 < $(PARAM_TEST) > param_90.out
	./$(EJ) tamaño parametro 95 < $(PARAM_TEST) > param_95.out
	./$(EJ) tamaño parametro 100 < $(PARAM_TEST) > param_100.out
	
test_param_count: $(EJ)
	./$(EJ) count parametro 5 < $(PARAM_TEST) > param_5.count
	./$(EJ) count parametro 10 < $(PARAM_TEST) > param_10.count
	./$(EJ) count parametro 15 < $(PARAM_TEST) > param_15.count
	./$(EJ) count parametro 20 < $(PARAM_TEST) > param_20.count
	./$(EJ) count parametro 25 < $(PARAM_TEST) > param_25.count
	./$(EJ) count parametro 30 < $(PARAM_TEST) > param_30.count
	./$(EJ) count parametro 35 < $(PARAM_TEST) > param_35.count
	./$(EJ) count parametro 40 < $(PARAM_TEST) > param_40.count
	./$(EJ) count parametro 45 < $(PARAM_TEST) > param_45.count
	./$(EJ) count parametro 50 < $(PARAM_TEST) > param_50.count
	./$(EJ) count parametro 55 < $(PARAM_TEST) > param_55.count
	./$(EJ) count parametro 60 < $(PARAM_TEST) > param_60.count
	./$(EJ) count parametro 65 < $(PARAM_TEST) > param_65.count
	./$(EJ) count parametro 70 < $(PARAM_TEST) > param_70.count
	./$(EJ) count parametro 75 < $(PARAM_TEST) > param_75.count
	./$(EJ) count parametro 80 < $(PARAM_TEST) > param_80.count
	./$(EJ) count parametro 85 < $(PARAM_TEST) > param_85.count
	./$(EJ) count parametro 90 < $(PARAM_TEST) > param_90.count
	./$(EJ) count parametro 95 < $(PARAM_TEST) > param_95.count
	./$(EJ) count parametro 100 < $(PARAM_TEST) > param_100.count


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
