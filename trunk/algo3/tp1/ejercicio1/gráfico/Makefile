
EJ=bn_mod_n
TESTS_PATH=../tests
DATA_PATH=data

data := $(subst $(TESTS_PATH),$(DATA_PATH), $(patsubst %.in,%.dat,$(wildcard $(TESTS_PATH)/*.in)))

data_count := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/count_%.dat,$(data))
data_time := $(patsubst $(DATA_PATH)/%.dat,$(DATA_PATH)/time_%.dat,$(data))

all: counts time

counts: $(data_count) 
	./plot_count

time: $(data_time)
	./plot_time

$(DATA_PATH)/time_%.dat:$(TESTS_PATH)/%.in
	../$(EJ) time < $< > $@

$(DATA_PATH)/count_%.dat: $(TESTS_PATH)/%.in
	../$(EJ) count < $< > $@

copy:
	cp png/*.png ../../informe/$(EJ)/
