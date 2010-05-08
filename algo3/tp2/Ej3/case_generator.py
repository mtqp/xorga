#!/usr/bin/python

from random import randint, seed, choice

def generar_random(casos, max_habitaciones):
	fIn = open('test.in','w')
	for i in range(casos):
		habitaciones = randint(4,max_habitaciones)
		max_pasillos = (habitaciones*(habitaciones-1))/2
		pasillos = randint(0,max_pasillos)
		llaves = randint(0,(habitaciones-2)/2)
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
	fIn.close()
	
def generar_libre(casos, max_habitaciones):
	fIn = open('test_libre.in', 'w')
	for i in range(casos):
		habitaciones = randint(4,max_habitaciones)
		max_pasillos = (habitaciones*(habitaciones-1))/2
		pasillos = randint(habitaciones-1,max_pasillos)
		llaves = randint(0,(habitaciones-2)/2)
		fIn.write("%s %s %s\n"%(habitaciones, llaves, pasillos))
		h = [x for x in range(2,habitaciones)]
		puertas = []
		faltan = []
		for j in range(habitaciones):
			puertas += [0]
			faltan += [j+1]
		for j in range(llaves):
			esta = choice(h)
			h.remove(esta)
			abre = choice(h)
			h.remove(abre)
			puertas[abre-1] = esta
			fIn.write("  %s %s\n"%(esta, abre))
		extremos = [x+1 for x in range(habitaciones)]
		extremos2 = [1]
		faltan.remove(1)
		candidatos_a_extremos = [[y for y in extremos if y!=x+1] for x in range(habitaciones)]
		j=0;
		#para asegurarme que quede conexo
		while len(faltan)>0 :
			extremo1 = choice(extremos2)
			extremo2 = choice(faltan)
			while puertas[extremo2-1]>0 and (puertas[extremo2-1] not in extremos2):
				extremo2 = choice(faltan)
			extremos2.append(extremo2)
			faltan.remove(extremo2)
			j=j+1
			if len(candidatos_a_extremos[extremo1-1])==1: extremos.remove(extremo1)
			else: candidatos_a_extremos[extremo1-1].remove(extremo2)
			if len(candidatos_a_extremos[extremo2-1])==1: extremos.remove(extremo2)
			else: candidatos_a_extremos[extremo2-1].remove(extremo1)
			fIn.write("    %s %s\n"%(extremo1, extremo2))
		#pongo el resto de las aristas	
		for j in range(pasillos-j):
			extremo1 = choice(extremos)
			extremo2 = choice(candidatos_a_extremos[extremo1-1])
			if len(candidatos_a_extremos[extremo1-1])==1: extremos.remove(extremo1)
			else: candidatos_a_extremos[extremo1-1].remove(extremo2)		
			if len(candidatos_a_extremos[extremo2-1])==1: extremos.remove(extremo2)
			else: candidatos_a_extremos[extremo2-1].remove(extremo1)
			fIn.write("    %s %s\n"%(extremo1, extremo2))
	fIn.write("-1 -1 -1")
	fIn.close()
	
def main():
	seed()
	
	generar_random(1000, 100);
	generar_libre(1000,100);

if __name__=="__main__":
	main()
