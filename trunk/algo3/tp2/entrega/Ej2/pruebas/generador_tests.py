#!/usr/bin/python

import random
import sys

def ciclo_minimo(n):
	#genero un ciclo basico de N nodos
	adyacencias = [ [i-1,i+1] for i in range(2,n) ]
	adyacencias = [[2,n]] + adyacencias + [[n-1,1]]
	return adyacencias

def todos_con_todos(n):
	adyacencias = [ [ x for x in range(1,n+1) if x!=i  ] for i in range(1,n+1) ]
	return adyacencias

def ciclo_random(n):
	ciclo = [ [] for i in range(n) ]
	for i in range(1,n):
		ciclo[i-1] += [ k for k in range(i+1,n+1) if k not in ciclo[i-1] and random.randint(1,4)==1 ]
	for i in range(1,n+1):
		ciclo[i-1] = [ k for k in range(1,n+1) if k in ciclo[i-1] or i in ciclo[k-1] ]
	for i in range(1,n+1):
		if i-1 not in ciclo[i-1] and i!=1:
			ciclo[i-1].append(i-1)
		if i+1 not in ciclo[i-1] and i!=n:
			ciclo[i-1].append(i+1)
		ciclo[i-1].sort()
	if n not in ciclo[0]:
		ciclo[0].append(n)
	if 1 not in ciclo[n-1]:
		ciclo[n-1].append(1)
	return ciclo


def grafo_random( n ):
	grafo = [ [] for i in range(n) ]
	for i in range(n):
		grafo[i] += [ x+1 for x in range(i)     if i+1 in grafo[x] ]
		grafo[i] += [ x+1 for x in range(i+1,n) if random.randint(0,15) == 1 ]
	return grafo

def mejor_caso_conexo( n ):
	grafo = [ [ i+1 for i in x ] for x in grafo_random(n-2) ]
	grafo.insert( 0, [2,n] )
	grafo.append( [1] );
	return grafo

def mejor_caso( n ):
	grafo = [ [ i+1 for i in x ] for x in grafo_random(n-1) ]
	grafo.insert( 0, [] )
	return grafo

def unir_ciclos(ciclo, ciclo2, fc=True):
	max = len(ciclo)
	desde = random.randint(0,max-1)
	hasta = random.randint(0,max-1)
	ciclo[desde] += [max+1]
	if fc: ciclo[hasta] += [max+len(ciclo2)]
	ciclo += [ [ i+max  for i in x ] for x in ciclo2  ]
	ciclo[max] += [desde+1]
	if fc: ciclo[max+len(ciclo2)-1] += [hasta+1]
	return ciclo


def generar_input(nombre, ciclos):
	f = open(nombre,'w')
	for ciclo in ciclos:
		f.write( '%d\n'%( len(ciclo) ))
		if len(ciclo) > 0:
			for i in range(len(ciclo)):
				if len(ciclo[i]) > 0:
					s = reduce( lambda x,y: "%s %s"%(x,y), ciclo[i] )
				else:
					s = ""
				f.write('%d %s\n'%( len(ciclo[i]), s) )
	f.write("-1")
	f.close()

def generar_instancias( instancias, fc=True ):
	generadores = [ todos_con_todos ]
	ciclos = []
	for i in instancias:
		instancia = []
		for c in i:
			if type(c)==type(tuple()):
				n,gen = c
			else:
				n = c
				gen = random.choice(generadores)
			ciclo = gen(n)
			if len(instancia)>0:
				unir_ciclos(instancia,ciclo,fc)
			else:
				instancia=ciclo
		ciclos+=[instancia]
	return ciclos

def graphviz( nombre, grafo ):
	f = open(nombre,'w')
	f.write( "graph G { \n" )
	for i in  range(len(grafo)):
		for j in grafo[i]:
			if j > i:
				f.write("%d -- %d;\n"%(i+1,j))
	f.write( "}" )

if __name__=="__main__":
	#ciclo = unir_ciclos(ciclo_minimo(3),ciclo_minimo(4))
	#print unir_ciclos(ciclo,todos_con_todos(5))
	instancias = []
	cantidad_instancias = 100
	print "Cantidad de instancias: %d"%(cantidad_instancias*3)
	print "Eligiendo instancias para generar",
	for n in range(15,15+cantidad_instancias):
		k = [ [a,b,c] for a in range(random.randint(3,n-10),n) for b in range(3,n) for c in range(3,n) if a+b+c == n ]
		s = random.sample(k,3)
		instancias += s
		sys.stdout.write( "." )
		sys.stdout.flush()
	print "OK"
	print "Generando grafos fuertemente conexos...",
	ciclos=generar_instancias( instancias )
	print "OK"
	print "Generando input...",
	generar_input( "test_fc.in", ciclos )
	print "OK"
	
	print "Generando grafos no fuertemente conexos...",
	ciclos=generar_instancias( instancias, False )
	print "OK"
	print "Generando input...",
	generar_input( "test_nofc.in", ciclos )
	print "OK"

	#instancias = [ [(x,mejor_caso)] for x in range(1,100) ]
	#ciclos=generar_instancias( instancias )
	#generar_input( "test_mejor_caso.in", ciclos )
	
