EJ = ciudad

all: $(EJ)

pruebas/%.in:
	pruebas/generador_tests.py
	mv test_fc.in pruebas/
	mv test_nofc.in pruebas/

diffs: pruebas/test_fc.in pruebas/test_nofc.in
	./$(EJ) < pruebas/test_fc.in | grep -c fuertemente\ conexo
	./$(EJ) < pruebas/test_nofc.in | grep -c no

diffs_clean:
	rm pruebas/test_fc.in
	rm pruebas/test_nofc.in

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
