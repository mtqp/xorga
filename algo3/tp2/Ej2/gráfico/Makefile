
EJ=ciudad
TESTS_PATH=../tests
DATA_PATH=data
OUT_PATH=ps

data := $(subst $(TESTS_PATH),$(DATA_PATH), $(patsubst %.in,%.dat,$(wildcard $(TESTS_PATH)/*.in)))

data_count := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/count_%.dat,$(data))
pdf_count := $(patsubst %.dat,%.pdf,$(subst $(DATA_PATH),$(OUT_PATH),$(data_count)) )

all: counts

count_%.eps: $(data_count) 
	./plot_count

%.pdf: %.eps
	ps2pdf $< $(patsubst %.eps,%.pdf,$<)

$(DATA_PATH)/count_%.dat: $(TESTS_PATH)/%.in
	../$(EJ) count < $< > $@

counts: plot_count $(data_count) $(pdf_count)

copy:
	cp ps/*.pdf ../../informe/$(EJ)/

clean:
	rm -f ps/*.pdf

clean_all: clean
	rm -f data/*.dat
