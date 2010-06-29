#!/usr/bin/python

def cargarSolucion( archivo, step=1 ):
	f = open( archivo, 'r' )
	solucion = []
	i=0
	for linea in f:
		if i%step == 0:
			solucion.append(int(linea.strip()))
		i+=1
	f.close()
	return solucion

heuristica = "constructiva"

exacto = cargarSolucion( 'exacto.out', 2 )
heuris = cargarSolucion( '%s.out'%(heuristica), 2 )

exacto_count = cargarSolucion( 'exacto.count' )
heuris_count = cargarSolucion( '%s.count'%(heuristica) )

f = open( 'union.dat','w' )
for i in range( len(exacto) ):
	f.write( '%d\t%d %d\t%d %d\n'%( i, exacto[i], exacto_count[i], heuris[i], heuris_count[i] ) )
f.close()
