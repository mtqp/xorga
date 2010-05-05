#!/usr/bin/python

import random

def ciclo_minimo(n):
	#genero un ciclo basico de N nodos
	adyacencias = [ [i-1,i+1] for i in range(2,n) ]
	adyacencias = [[2,n]] + adyacencias + [[n-1,1]]
	return adyacencias

def todos_con_todos(n):
	adyacencias = [ [ x for x in range(1,n+1) if x!=i  ] for i in range(1,n+1) ]
	return adyacencias

def ciclo_random(n):
	ciclo = ciclo_minimo(n)
	ciclo = [ [ x for x in range(1,n+1) if (x in ciclo[i-1]) or (x!=i and random.randint(0,1)==1) ] for i in range(1,n+1) ]
	return ciclo

def unir_ciclos(ciclo, ciclo2):
	max = len(ciclo)
	desde = random.randint(0,max-1)
	hasta = random.randint(0,max-1)
	ciclo[desde] += [max+1]
	ciclo[hasta] += [max+len(ciclo2)]
	ciclo += [ [ i+max  for i in x ] for x in ciclo2  ]
	ciclo[max] += [desde+1]
	ciclo[max+len(ciclo2)-1] += [hasta+1]
	return ciclo

def generar_input(ciclo):
	f = open('test_fc.in','w')
	f.write( '%d\n'%( len(ciclo) ))
	for i in range(len(ciclo)):
		s = reduce( lambda x,y: "%s %s"%(x,y), ciclo[i] )
		f.write('%d %s\n'%( len(ciclo[i]), s) )
	f.write("-1")
	f.close()

if __name__=="__main__":
	#ciclo = unir_ciclos(ciclo_minimo(3),ciclo_minimo(4))
	#print unir_ciclos(ciclo,todos_con_todos(5))
	ciclo= ciclo_random(18)
	print ciclo
	generar_input(ciclo)
