#!/usr/bin/python

import sys
import os.path
from tp3 import cargarInput, cargarOutput, lineaInstancia, graphviz, guardarInput

if __name__=="__main__":
	if len(sys.argv) == 4:
		entrada = sys.argv[1]
		metodo  = sys.argv[2]
		losquequiero = int(sys.argv[3])
	else:
		print "Uso: ",sys.argv[0]," {archivo_entrada} {metodo} {tamanyo_clique}"
		exit(1)
	print "Cargando input..",
	sys.stdout.flush()
	instancias  = cargarInput( entrada )
	print "OK"
	archivo     = os.path.basename(entrada)[0:-3]
	print "Cargando output..",
	sys.stdout.flush()
	soluciones  = cargarOutput( "%s/%s.out"%(metodo,archivo) )
	seleccionadas = []
	print "OK"
	print "Buscando instancias...",
	sys.stdout.flush()
	for i in range(len(soluciones)):
		if len(soluciones[i]) == losquequiero :
			seleccionadas.append(i)
	print "OK"
	print "Escribiendo nuevo input"
	sys.stdout.flush()
	instancias_seleccionadas = [ instancias[i]  for i in seleccionadas ]
	guardarInput( "%s/%s_%d.in"%(metodo,archivo,losquequiero), instancias_seleccionadas)
	soluciones_seleccionadas = [ soluciones[i] for i in seleccionadas ]
	#graphviz( "%s/%s.dot"%(metodo,archivo), instancias_seleccionadas, soluciones_seleccionadas )
