#!/usr/bin/python
from tp3 import grafo_random, guardarInput, grafoCliqueMin

instancias=[]
for i in range(1,801,8):
	print "creando instancia %d.."%(i),
	instancias.append( grafo_random( i ) )
	print "ok"
guardarInput( 'test/grafo_random_1a800.in', instancias )
#guardarInput( 'test/grafo_random_1a100.in', instancias )
#guardarInput( 'test/grafo_random_n_fijo_clique.in', instancias )

