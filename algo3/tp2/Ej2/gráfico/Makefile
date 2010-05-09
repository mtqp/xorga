
EJ=ciudad
TESTS_PATH=../tests
DATA_PATH=data
OUT_PATH=ps

data := $(subst $(TESTS_PATH),$(DATA_PATH), $(patsubst %.in,%.dat,$(wildcard $(TESTS_PATH)/*.in)))

data_count := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/count_%.dat,$(data))
data_time := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/time_%.dat,$(data))
pdf_count := $(patsubst %.dat,%.pdf,$(subst $(DATA_PATH),$(OUT_PATH),$(data_count)) )
pdf_time := $(patsubst %.dat,%.pdf,$(subst $(DATA_PATH),$(OUT_PATH),$(data_time)) )

all: counts time

time_%.eps: $(data_time)
	./plot_time

count_%.eps: $(data_count) 
	./plot_count

%.pdf: %.eps
	ps2pdf $< $(patsubst %.eps,%.pdf,$<)

$(DATA_PATH)/time_%.dat:$(TESTS_PATH)/%.in
	../$(EJ) time < $< > $@

$(DATA_PATH)/count_%.dat: $(TESTS_PATH)/%.in
	../$(EJ) count < $< > $@

counts: plot_count $(data_count) $(pdf_count)
time: plot_time $(data_time) $(pdf_time)

copy:
	cp ps/*.pdf ../../informe/$(EJ)/

clean:
	rm ps/*.pdf

clean_all: clean
	rm data/*.dat
