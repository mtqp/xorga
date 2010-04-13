ARCHIVOS = bn_mod_n
CFLAGS = -Wall

all: $(ARCHIVOS)

%:%.cpp ../medicion.h
	g++ -o $@ $< $(CFLAGS)

clean:
	rm $(ARCHIVOS)