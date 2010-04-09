ARCHIVOS = bn_mod_n
CFLAGS = -g -Wall

all: $(ARCHIVOS)

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

clean:
	rm $(ARCHIVOS)
