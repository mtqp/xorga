#!/usr/bin/python

import sys
import os.path
from tp3 import cargarInput, cargarOutput, lineaInstancia

if __name__=="__main__":
	if len(sys.argv) == 4:
		entrada = sys.argv[1]
		metodo1 = sys.argv[2]
		metodo2 = sys.argv[3]
	else:
		print "Uso: ",sys.argv[0]," {archivo_entrada} {metodo1} {metodo2}"
		exit(1)
	instancias  = cargarInput( entrada )
	soluciones1 = cargarOutput( "%s/%s.out"%(metodo1,os.path.basename(entrada)[0:-3]) )
	soluciones2 = cargarOutput( "%s/%s.out"%(metodo2,os.path.basename(entrada)[0:-3]) )
	for i in range(len(soluciones1)):
		if len(soluciones1[i]) != len(soluciones2[i]) :
			print "La %da instancia difiere - tamanyo %s: %d  tamanyo %s: %d"%(i+1,metodo1,len(soluciones1[i]),metodo2,len(soluciones2[i]))
			print "\t %s: %s"%(metodo1,str(soluciones1[i]))
			print "\t %s: %s"%(metodo2,str(soluciones2[i]))
			print "\t Linea: %d"%(lineaInstancia(instancias,i))
			print "-------------------------------------------------------------"
