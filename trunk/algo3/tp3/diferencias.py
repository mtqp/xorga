#!/usr/bin/python

import sys
import os.path
from tp3 import cargarInput, cargarOutput, lineaInstancia, graphviz

if __name__=="__main__":
	if len(sys.argv) == 4:
		entrada = sys.argv[1]
		metodo1 = sys.argv[2]
		metodo2 = sys.argv[3]
	else:
		print "Uso: ",sys.argv[0]," {archivo_entrada} {metodo1} {metodo2}"
		exit(1)
	instancias  = cargarInput( entrada )
	archivo     = os.path.basename(entrada)[0:-3]
	soluciones1 = cargarOutput( "%s/%s.out"%(metodo1,archivo) )
	soluciones2 = cargarOutput( "%s/%s.out"%(metodo2,archivo) )
	if len(soluciones1) != len(soluciones2) :
		print "La cantidad de instancias de los archivos de soluciones son diferentes.\nHay %d soluciones en %s y %d soluciones en %s."%(len(soluciones1),metodo1,len(soluciones2,metodo2))
		exit(1)
	diferentes  = []
	for i in range(len(soluciones1)):
		if len(soluciones1[i]) != len(soluciones2[i]) :
			diferentes.append( i )
			print "La %da instancia difiere - tamanyo %s: %d  tamanyo %s: %d"%(i+1,metodo1,len(soluciones1[i]),metodo2,len(soluciones2[i]))
			print "\t %s: %s"%(metodo1,str(soluciones1[i]))
			print "\t %s: %s"%(metodo2,str(soluciones2[i]))
			print "\t Linea: %d"%(lineaInstancia(instancias,i))
			print "-------------------------------------------------------------"
	instancias_diferentes = [ instancias[i]  for i in diferentes ]
	soluciones_diferentes = [ soluciones1[i] for i in diferentes ]
	graphviz( "%s/%s.dot"%(metodo1,archivo), instancias_diferentes, soluciones_diferentes )
	soluciones_diferentes = [ soluciones2[i] for i in diferentes ]
	graphviz( "%s/%s.dot"%(metodo2,archivo), instancias_diferentes, soluciones_diferentes )

