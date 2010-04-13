EJ = ronda_de_amigas
GEN = cagen_densidad.py

ejercicio: $(EJ)

%: %.cpp
	g++ -o $@ $< -Wall

graf_counts:
	./$(GEN)
	./$(EJ) count < test.in > counts.dat
	cp counts.dat gej2/
	gej2/plot_counts

clean:
	rm ronda_de_amigas

