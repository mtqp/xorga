SOURCES=informe.tex
OBJS= ./otros/*

all: informe

informe: $(SOURCES) $(OBJS)
	pdflatex $^
	pdflatex $^	
	mv -f  *.toc *.aux *.idx *.log	./logs
	clear; echo 'PDF COMPILADO!'
        
clean:
	rm -f .logs/*
	rm -f ipc.pdf
	
new: clean all
	
