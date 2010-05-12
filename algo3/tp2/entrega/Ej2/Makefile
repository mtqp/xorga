EJ = ciudad

all: $(EJ)

pruebas/%.in:
	@echo "--- Generando casos de prueba ---"
	@pruebas/generador_tests.py
	@mv test_fc.in pruebas/
	@mv test_nofc.in pruebas/

diffs: $(EJ) pruebas/test_fc.in pruebas/test_nofc.in
	@echo "--- Ejecutando casos de prueba ---"
	@echo "Cantidad de grafos fuertemente conexos en test_fc.in:"
	@./$(EJ) < pruebas/test_fc.in | grep -c fuertemente\ conexo
	@echo "Cantidad de grafos no fuertemente conexos en test_nofc.in:"
	@./$(EJ) < pruebas/test_nofc.in | grep -c no

diffs_clean:
	rm pruebas/test_fc.in
	rm pruebas/test_nofc.in

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
