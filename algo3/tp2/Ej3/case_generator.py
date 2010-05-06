#!/usr/bin/python

from random import randint, seed, choice

casos = 10
max_habitaciones = 10

seed()

fIn = open('test.in','w')

for i in range(casos):
	habitaciones = randint(4,max_habitaciones)
	max_pasillos = (habitaciones*(habitaciones-1))/2
	pasillos = randint(0,max_pasillos)
	llaves = randint(0,(habitaciones-2)/2)
	#if (llaves<0): llaves=0
	fIn.write("%s %s %s\n"%(habitaciones, llaves, pasillos))
	h = [x for x in range(2,habitaciones)]
	for j in range(llaves):
		esta = choice(h)
		h.remove(esta)
		abre = choice(h)
		h.remove(abre)
		fIn.write("  %s %s\n"%(esta, abre))
	extremos = [x+1 for x in range(habitaciones)]
	candidatos_a_extremos = [[y for y in extremos if y!=x+1] for x in range(habitaciones)]
	for j in range(pasillos):
		extremo1 = choice(extremos)
		extremo2 = choice(candidatos_a_extremos[extremo1-1])
		if len(candidatos_a_extremos[extremo1-1])==1: extremos.remove(extremo1)
		else: candidatos_a_extremos[extremo1-1].remove(extremo2)		
		if len(candidatos_a_extremos[extremo2-1])==1: extremos.remove(extremo2)
		else: candidatos_a_extremos[extremo2-1].remove(extremo1)
		fIn.write("    %s %s\n"%(extremo1, extremo2))
fIn.write("-1 -1 -1")
