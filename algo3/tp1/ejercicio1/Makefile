ARCHIVOS = bn_mod_n
CFLAGS = -Wall

all: $(ARCHIVOS)

data: test_if.dat test_no_if.dat

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

%.dat:%.in
	./bn_mod_n < $@ > gráficos/data/$<

clean:
	rm $(ARCHIVOS)
