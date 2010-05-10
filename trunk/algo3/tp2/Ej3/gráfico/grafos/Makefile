GRAFOS=peor_caso.eps

all: $(GRAFOS)

%.eps:%.grafo
	dot -Tps $< > $@

clean:
	rm peor_caso.ps
