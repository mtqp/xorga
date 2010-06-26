
all: informe.pdf

informe.pdf: informe.tex exacto.tex constructiva.tex busqueda_local.tex busqueda_tabu.tex
	latex informe.tex dvips -p pdf
	dvipdf informe.dvi informe.pdf
	rm informe.dvi
#	pdflatex informe.tex

clean:
	rm informe.pdf
