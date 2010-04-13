#!/usr/bin/python
# Case Generator Densidad

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

def main():
	seed()
	
	densidad = 0.7
	min_amigos = 2
	cantidad_por_n = 20
	
	fIn = open("test.in",'w')
	for n in range(6,19):
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
	main()
	
