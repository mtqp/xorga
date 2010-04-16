ARCHIVOS = secuencia_unimodal
CFLAGS = -Wall

all: $(ARCHIVOS)

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

clean:
	rm $(ARCHIVOS)
