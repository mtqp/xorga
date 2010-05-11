#!/usr/bin/python

from random import randint, seed, choice, uniform

def generar_random_por_n(habitaciones, densidad):
	max_pasillos = int(((habitaciones*(habitaciones-1))/2) * densidad)
	n_pasillos = randint(max_pasillos,max_pasillos)
	llaves = randint(0,(habitaciones-2)/2)
	h = [x for x in range(2,habitaciones)]
	puertas = []
	for j in range(llaves):
		esta = choice(h)
		h.remove(esta)
		abre = choice(h)
		h.remove(abre)
		puertas.append((esta,abre))
	extremos = [x+1 for x in range(habitaciones)]
	candidatos_a_extremos = [[y for y in extremos if y!=x+1] for x in range(habitaciones)]
	pasillos = []
	for j in range(n_pasillos):
		extremo1 = choice(extremos)
		extremo2 = choice(candidatos_a_extremos[extremo1-1])
		if len(candidatos_a_extremos[extremo1-1])==1: extremos.remove(extremo1)
		else: candidatos_a_extremos[extremo1-1].remove(extremo2)		
		if len(candidatos_a_extremos[extremo2-1])==1: extremos.remove(extremo2)
		else: candidatos_a_extremos[extremo2-1].remove(extremo1)
		pasillos.append((extremo1,extremo2))
	return (puertas,pasillos)

def mejor_caso( n, caso ):
	mapa = generar_random_por_n(n-1,uniform(0,1))
	puertas, pasillos = mapa
	pasillos = [ (x+1,y+1) for (x,y) in pasillos ]
	return (n,puertas,pasillos)

def graphviz( nombre, mapa ):
	f = open(nombre,'w')
	f.write( "graph G { \n" )
	n,puertas,pasillos = mapa
	for i in range(n):
		f.write("%d;\n"%(i+1))
	for pasillo in pasillos:
		i,j = pasillo
		f.write("%d -- %d;\n"%(i,j))
	f.write( "}" )

def generar_input( nombre, instancias ):
	fIn = open(nombre,'w')
	for instancia in instancias:
		n,puertas,pasillos = instancia
		fIn.write( "%d %d %d\n"%(n,len(puertas),len(pasillos)) )
		for puerta in puertas:
			esta, abre = puerta
			fIn.write( "  %d %d\n"%(esta, abre) )
		for pasillo in pasillos:
			extremo1, extremo2 = pasillo
			fIn.write( "    %d %d\n"%(extremo1, extremo2) )
	fIn.write("-1 -1 -1")
	fIn.close()	

if __name__=="__main__":	
	instancias = []
	#graphviz( "random.dot", mejor_caso(10) )
	for i in range(4,100):
		instancias.append( mejor_caso(i) )
	generar_input( "test_mejor_caso.in", instancias )

