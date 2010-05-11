EJ = ciudad

all: $(EJ)

diffs:
	./$(EJ) < test/test_fc.in | grep -c fuertemente\ conexo

%:%.cpp
	g++ -o $@ $<

clean:
	rm $(EJ)
