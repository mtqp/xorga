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

def porc_err( cantidad, total ):
	return round(1.0-(float(s2)/float(s1)), 1)
	

def unir( metodo1, metodo2, archivo_salida, carpeta_data='data' ):
	s1 = cargarSolucion( '%s/%s.out'%(carpeta_data,metodo1) )
	s2 = cargarSolucion( '%s/%s.out'%(carpeta_data,metodo2) )

	count1 = cargarSolucion( '%s/%s.count'%(carpeta_data,metodo1), 1, 2 )
	count2 = cargarSolucion( '%s/%s.count'%(carpeta_data,metodo2), 1, 2 )

	f = open( archivo_salida,'w' )
	for i in range( len(s1) ):
		#f.write( '%d\t%d %d\t%d %d\t%f\n'%( i, s1[i], count1[i], s2[i], count2[i], porc_err(s1[i],s2[i]),  ) )
		f.write( '%d\t%d %d\t%d %d\n'%( i, s1[i], count1[i], s2[i], count2[i] ) )
	f.close()


for p in range(5,105,5):
	heuristica='param_%d'%(p)
	unir( 'exacto', heuristica, 'data/%s.dat'%(heuristica) )

