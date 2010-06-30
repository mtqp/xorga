ALGOS=constructivo busqueda_local tabu_search exacto

.PHONY: all

all: $(ALGOS)
	@make -C constructivo
	@make -C busqueda_local
	@make -C tabu_search
	@make -C exacto

clean:
	@make -C constructivo clean
	@make -C busqueda_local clean
	@make -C tabu_search clean
	@make -C exacto clean
