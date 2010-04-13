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
				cuantos_saco = min( randint(0, faltan), len(amistades[i])-min_amigos )
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

seed()
n = 8
densidad = 0.5
min_amigos = 2

for n in range(8,100):
	amistades = generar_amistades(n,densidad,min_amigos)

#for i in range(n): print "%d: %s"%(i+1,amistades[i])

