EJ = ciudad

all: $(EJ)

test/%.in:
	test/generador_tests.py
	mv test_fc.in test/
	mv test_nofc.in test/

diffs: test/test_fc.in test/test_nofc.in
	./$(EJ) < test/test_fc.in | grep -c fuertemente\ conexo
	./$(EJ) < test/test_nofc.in | grep -c no

diffs_clean:
	rm test/test_fc.in
	rm test/test_nofc.in

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
