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

def generar_input(ciclos):
	f = open('test_fc.in','w')
	for ciclo in ciclos:
		f.write( '%d\n'%( len(ciclo) ))
		for i in range(len(ciclo)):
			s = reduce( lambda x,y: "%s %s"%(x,y), ciclo[i] )
			f.write('%d %s\n'%( len(ciclo[i]), s) )
	f.write("-1")
	f.close()

def generar_instancias( instancias ):
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
				unir_ciclos(instancia,ciclo)
			else:
				instancia=ciclo
		ciclos+=[instancia]
	return ciclos


if __name__=="__main__":
	#ciclo = unir_ciclos(ciclo_minimo(3),ciclo_minimo(4))
	#print unir_ciclos(ciclo,todos_con_todos(5))
	n = 15
	instancias = []
	for i in range(n):
		a = random.randint(3,n-5)
	 	b = random.randint(3,n-3-a)
		c = n-a-b
		instancias += [[a,b,c]]
	ciclos=generar_instancias( instancias  )
	generar_input( ciclos )
	
