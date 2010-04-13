EJ = ronda_de_amigas
GEN = cagen_densidad.py

ejercicio: $(EJ)

%: %.cpp
	g++ -o $@ $< -Wall

graf_counts:
	python $(GEN)
	./$(EJ) count < test.in > counts.dat
	cp counts.dat ../informe/gej2/
	../informe/gej2/plot_counts

clean:
	rm ronda_de_amigas

