#!/usr/bin/python

import subprocess
import sys
import os
from tp3 import cargarCounts, cargarInput, cargarOutput, graphviz

def max_counts( archivo ):
	counts = cargarCounts(archivo)
	enes   = set( [ x[1] for x in counts ] )
	maximos = []
	for n in enes:
		instancias = [ x for x in counts if x[1] == n ]
		maximo = max( instancias, key=lambda x: x[1] )
		maximos.append( maximo )	
#	counts = array_plano( maximos )
#	for instancia in maximos:
#		if type(instancia) is list:
#			counts += instancia
#		else:
#			counts.append(instancia)
	return maximos


if __name__=="__main__":
	if len(sys.argv) == 3:
		entrada = sys.argv[1]
		metodo  = sys.argv[2]
		archivo = metodo+'/'+os.path.basename(entrada)[0:-3]
		if not os.path.exists(archivo+'.count'):
			print "%s.count no existe"%(archivo)
			exit(1)
		if not os.path.exists(entrada):
			print "%s no existe"%(entrada)
			exit(1)
		if not os.path.exists(archivo+'.out'):
			print "%s.out no existe"%(archivo)
			exit(1)
	else:
		print "Uso: ",sys.argv[0]," {archivo_entrada} {metodo}"
		exit(1)
	interesantes = max_counts( "%s.count"%(archivo) )
	instancias   = cargarInput( entrada )
	soluciones   = cargarOutput( "%s.out"%(archivo) )
	instancias_interesantes = [ instancias[instancia[0]] for instancia in interesantes ]
	soluciones_interesantes = [ soluciones[instancia[0]] for instancia in interesantes ]
	graphviz( "%s.dot"%(archivo), instancias_interesantes, soluciones_interesantes )
