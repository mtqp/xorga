#!/usr/bin/python

import sys
from random import randint, seed, choice, uniform

def grafos_random(n):
	nodos = [[] for x in range(n)]
	candidatos = [[y for y in range(n) if y!=x] for x in range(n)]
	for i in range(n):
		cant_ady = randint(0,len(candidatos[i]))
		cant_ady-= randint(0, cant_ady*2)
		if cant_ady < 0: cant_ady = 0
		for j in range(cant_ady):
			ady = choice(candidatos[i])
			nodos[i].append(ady)
			nodos[ady].append(i)
			candidatos[i].remove(ady)
			candidatos[ady].remove(i)
	return(n,nodos)

def grafos_max_clique(n,maxima):
	nodos = [[] for x in range(n)]
	vertices = [x for x in range(n)]
	max_clique = []
	for i in range(maxima):
		vertice = choice(vertices)
		max_clique.append(vertice)
		vertices.remove(vertice)
		for v in max_clique:
			nodos[v] = [x for x in max_clique if x!=v]
	candidatos = [[y for y in range(n) if (y!=x) and not(x in max_clique and y in max_clique)] for x in range(n)]
	for i in range(n):
		if i not in max_clique:
			#cant_ady = randint(0,min(len(candidatos[i]), maxima-1))
			cant_ady = len(candidatos[i])
			if cant_ady+len(nodos[i])>=maxima:
				cant_ady = maxima - len(nodos[i])
			for j in range(cant_ady):
				ady = choice(candidatos[i])
				nodos[i].append(ady)
				nodos[ady].append(i)
				candidatos[i].remove(ady)
				candidatos[ady].remove(i)
			for candidato in candidatos:
				if i in candidato:
					candidato.remove(i)
	return(n,nodos,maxima,max_clique)

def generar_input(nombre, instancias):
	fIn = open(nombre,'w')
	for instancia in instancias:
		n, nodos = instancia[0:2]
		fIn.write("%d\n"%n)
		for nodo in nodos:
			fIn.write("%d "%len(nodo))
			for v in nodo:
			 	fIn.write("%d "%(v+1))
	 		fIn.write("\n")
	fIn.write("-1")
	fIn.close()
	
def generar_output(nombre, instancias):
	fOut = open(nombre, 'w')
	f = open("tamano.out", 'w')
	for instancia in instancias:
		maxima, max_clique = instancia[2:4]
		fOut.write("%d\nN"%maxima)
		f.write("%d\n"%maxima)
		for v in max_clique:
			fOut.write(" %d"%(v+1))
		fOut.write("\n")
	fOut.close()
	f.close()

if __name__=="__main__":
	tam = sys.argv[1]
	#tam = raw_input("")
	tamanyo = int(tam)+1	
	#cant = raw_input("")
	cant = sys.argv[2]
	cantidad = int(cant)
	
	instancias_menores = raw_input("Presione 1 para generar todas las instancias menores, presione 0 si quiere hacer solo esa instancia.\n")
	instancias_menores = int(instancias_menores)
	instancias = []
	instancias_max = []
	
	if(instancias_menores == 1):
		for i in range(2,tamanyo):
			for k in range(cantidad):
				instancias.append(grafos_random(i))
			for j in range(1,i+1):
				for k in range(cantidad):
					instancias_max.append(grafos_max_clique(i,j))
	else:
		if(instancias_menores == 0):
			for k in range(cantidad):
				print "Calculando grafo random numero ", k
				instancias.append(grafos_random(tamanyo))
			'''print "Calculando grafos Max_clique"
			for k in range(cantidad):
				instancias_max.append(grafos_max_clique(tamanyo,15))'''
		else:
			print "No es opcion valida."
	
	print "Generando Input"
	generar_input("test/test_random.in", instancias)
	generar_input("test/test_max_clique.in", instancias_max)
	#generar_output("test_max_clique.out", instancias_max)
