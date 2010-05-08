ARCHIVOS = secuencia_unimodal
CFLAGS = -Wall -O3

all: $(ARCHIVOS)

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

clean:
	rm $(ARCHIVOS)
