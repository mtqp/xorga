GRAFOS=peor_caso.eps peor_caso_impar.eps

all: $(GRAFOS)

%.eps:%.grafo
	dot -Tps $< > $@

clean:
	rm $(GRAFOS)

copy:
	cp *.png ../../../informe/prision/

