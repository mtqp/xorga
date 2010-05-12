EJ = prision

all: $(EJ)

pruebas/test_libre.in:
pruebas/test_no_libre.in:
	pruebas/generador_tests.py
	mv test_libre.in pruebas/
	mv test_no_libre.in pruebas/

diffs:pruebas/test_libre.in pruebas/test_no_libre.in
	./$(EJ) < pruebas/test_libre.in | grep -c libre
	./$(EJ) < pruebas/test_no_libre.in | grep -c no

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
