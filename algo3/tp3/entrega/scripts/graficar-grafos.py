#!/usr/bin/python

import sys
from tp3 import cargarInput, cargarOutput, graphviz

if __name__=="__main__":
	if len(sys.argv) == 3:
		entrada     = sys.argv[1]
		entrada_out = sys.argv[2]
	else:
		entrada    = 'Tp3.in'
		entrada_out= 'Tp3exacto.out'
	instancias = cargarInput( entrada )
	soluciones = cargarOutput( entrada_out )
	for i in range(len(instancias)):
		graphviz( entrada_out+'.dot', instancias, soluciones )

