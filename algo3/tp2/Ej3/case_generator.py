#!/usr/bin/python

from random import randint, seed, choice

casos = 6
max_esquinas = 7

seed()

fIn = open('test.in','w')

for i in range(casos):
	esquinas = randint(4,max_esquinas)
	max_calles = (esquinas*(esquinas-1))/2
	calles = randint(0,max_calles)
	llaves = (esquinas-2)/2
	if (llaves<0): llaves=0
	fIn.write("%s %s %s\n"%(esquinas, llaves, calles))
	habitaciones = [x for x in range(2,esquinas)]
	for j in range(llaves):
		esta = choice(habitaciones)
		habitaciones.remove(esta)
		abre = choice(habitaciones)
		habitaciones.remove(abre)
		fIn.write("  %s %s\n"%(esta, abre))
	extremos = [x+1 for x in range(esquinas)]
	candidatos_a_extremos = [[y for y in extremos if y!=x+1] for x in range(esquinas)]
	for j in range(calles):
		extremo1 = choice(extremos)
		extremo2 = choice(candidatos_a_extremos[extremo1-1])
		if len(candidatos_a_extremos[extremo1-1])==1: extremos.remove(extremo1)
		else: candidatos_a_extremos[extremo1-1].remove(extremo2)		
		if len(candidatos_a_extremos[extremo2-1])==1: extremos.remove(extremo2)
		else: candidatos_a_extremos[extremo2-1].remove(extremo1)
		fIn.write("    %s %s\n"%(extremo1, extremo2))
fIn.write("-1 -1 -1")
