
EJ=ciudad
TESTS_PATH=../tests
DATA_PATH=data
OUT_PATH=ps

data := $(subst $(TESTS_PATH),$(DATA_PATH), $(patsubst %.in,%.dat,$(wildcard $(TESTS_PATH)/*.in)))

data_count := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/count_%.dat,$(data))
pdf_count := $(patsubst %.dat,%.pdf,$(subst $(DATA_PATH),$(OUT_PATH),$(data_count)) )

all: counts

$(DATA_PATH)/count_%.dat: $(TESTS_PATH)/%.in
	../$(EJ) count < $< > $@

counts: plot_count $(data_count)
	./plot_count
	ps2pdf ps/count_completos.eps ps/count_completos.pdf
	ps2pdf ps/count_test_mejor_caso.eps ps/count_test_mejor_caso.pdf
	rm ps/*.eps

copy:
	cp ps/*.pdf ../../informe/$(EJ)/

clean:
	rm -f ps/*.pdf

clean_all: clean
	rm -f data/*.dat
