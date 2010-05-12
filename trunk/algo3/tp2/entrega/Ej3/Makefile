EJ = prision

all: $(EJ)

pruebas/test_libre.in:
pruebas/test_no_libre.in:
	@echo "--- Generando casos de prueba ---"
	@pruebas/generador_tests.py
	@mv test_libre.in pruebas/
	@mv test_no_libre.in pruebas/

diffs:$(EJ) pruebas/test_libre.in pruebas/test_no_libre.in
	@echo "--- Ejecutando casos de prueba ---"
	@echo "Cantidad de casos 'libre' en test_libre.in:"
	@./$(EJ) < pruebas/test_libre.in | grep -c libre
	@echo "Cantidad de casos 'no libre' en test_no_libre.in:"
	@./$(EJ) < pruebas/test_no_libre.in | grep -c no

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
