GRAFOS = $(patsubst %.dot, %.eps, $(wildcard *.dot ))

grafos: $(GRAFOS)

%.eps: %.dot
	@echo "Generando $@"
	@dot -Tps $< > $@
	@rm $<

clean:
	rm -f $(wildcard *.out.eps)

