#!/usr/bin/python
# Generador de test para el ejercicio 2

from optparse import OptionParser

from random import randint, seed, choice
import sys

def generar_amistades( n, densidad, min_amigos ):
	relaciones = int(n*(n-1)/2 * densidad)
	personas = set([x+1 for x in range(n)])
	amistades = [ set(personas) for x in range(n) ]
	faltan = n*(n-1)/2 - relaciones

	while faltan > 0:
		for i in range(n):
			amistades[i].discard(i+1)
			cuantos_saco = min( randint(0, faltan), max(len(amistades[i])-min_amigos,0) )
			candidatos = [ x for x in amistades[i] if len(amistades[x-1])>min_amigos ]
			if len(candidatos) < cuantos_saco:
				cuantos_saco = len(candidatos)
			for j in range(cuantos_saco): 
				no_amigo = choice(candidatos)
				amistades[no_amigo-1].discard(i+1)
				amistades[i].discard(no_amigo)
				candidatos.remove(no_amigo)
			faltan -= cuantos_saco

	return amistades

def main(options):
	seed()
	
	densidad = options.densidad
	min_amigos = options.min_amigos
	cantidad_por_n = options.count
	
	fIn = open(options.archivo+".in",'w')
	for n in range(options.min_n,options.max_n+1):
		for k in range(cantidad_por_n):
			amistades = generar_amistades(n,densidad,min_amigos)
			fIn.write("%d\n"%(n))
			for i in range(len(amistades)):
				if len(amistades[i]) > 0:
					fIn.write("%d %s\n"%(len(amistades[i]), reduce(lambda x,y: "%s %s"%(x,y),amistades[i]) ) )
				else:
					fIn.write("%d\n"%(len(amistades[i])))
	
	fIn.write("%d"%(-1))

if __name__=="__main__":
	parser = OptionParser()
	parser.add_option("-a", "--archivo", dest="archivo", help="escribe los test en ARCHIVO.in", metavar='ARCHIVO', default='test')
	parser.add_option("-c", "--count", dest="count", help="genera COUNT casos de prueba por cada n", type='int', default=20, metavar='COUNT')
	parser.add_option("-d", "--densidad", dest="densidad", help="establece la densidad de relaciones", type='float', default=0.5, metavar='DENSIDAD')
	parser.add_option("--min_n", dest="min_n", help="establece el rango de n desde MIN_N", type='int', default=6, metavar='MIN_N')
	parser.add_option("--max_n", dest="max_n", help="establece el rango de n hasta MAX_N", type='int', default=18, metavar='MAX_N')
	parser.add_option("--min_amigos", dest="min_amigos", help="establece el minimo de amigos por persona", type='int', default=2)
	(options, args) = parser.parse_args()
	main(options)
	
