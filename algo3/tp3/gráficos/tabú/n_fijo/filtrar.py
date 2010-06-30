#!/usr/bin/python

def cargarSolucion( archivo, step=1, columna=1 ):
	f = open( archivo, 'r' )
	solucion = []
	i=0
	for linea in f:
		if i%step == 0:
			fila = linea.split()
			solucion.append( int(fila[columna-1]) )
		i+=1
	f.close()
	return solucion

def contar( metodo, archivo_salida, carpeta_data='data' ):
	s = cargarSolucion( '%s/%s.out'%(carpeta_data,metodo) )
	s.sort()
	tamanyos = list( set(s) )
	tamanyos.sort()

	f = open( archivo_salida,'w' )
	for tamanyo in tamanyos:
		f.write( '%d\t%d\n'%( tamanyo, s.count(tamanyo) ) )
	f.close()

for p in range(5,105,5):
	heuristica='param_%d'%(p)
	contar( heuristica, 'data/%s.dat'%(heuristica) )

contar( 'exacto', 'data/exacto.dat' )
contar( 'param_200', 'data/param_200.dat' )
contar( 'param_300', 'data/param_300.dat' )

