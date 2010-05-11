#!/usr/bin/python

from random import randint, seed, choice
import sys

def generar_random(casos, habitaciones):
	fIn = open('test.in','w')
	for i in range(casos):
		habitaciones = randint(4,max_habitaciones)
		max_pasillos = int((habitaciones*(habitaciones-1))/2)
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
	
def generar_libre(casos, max_habitaciones):
	fIn = open('test_libre.in', 'w')
	for i in range(casos):
		habitaciones = randint(4,max_habitaciones)
		max_pasillos = int((habitaciones*(habitaciones-1))/2)
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
	
def generar_densidad(n,densidad, muestras_por_n=1):
	fIn = open('test_densidad_%.1f.in'%(densidad),'w')
	for k in range(4,n):
		for i in range(muestras_por_n):
			mapa=generar_random_por_n(k,densidad)
			puertas,pasillos = mapa
			fIn.write( "%d %d %d\n"%(k,len(puertas),len(pasillos)) )
			for puerta in puertas:
				esta, abre = puerta
				fIn.write( "  %d %d\n"%(esta, abre) )
			for pasillo in pasillos:
				extremo1, extremo2 = pasillo
				fIn.write( "    %d %d\n"%(extremo1, extremo2) )
	fIn.write("-1 -1 -1")
	fIn.close()	

def generar_no_libre( n ):
	mapa = generar_random_por_n(n-3,1)
	puertas, pasillos = mapa
	pasillos += [ (n-3,n-2), (n-2,n-1), (n-1,n) ]
	puertas.append( (n-1,n-2) )
	return (n,puertas,pasillos)

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


def main():
	seed()
	
	#generar_random(1000, 100);
	print "Generando instancias libre...",
	sys.stdout.flush()
	generar_libre(1000,100)
	print "OK"
	instancias = []
	print "Generando instancias no-libre",
	sys.stdout.flush()
	for i in range(6,106):
		instancias.append( generar_no_libre(i) )
		sys.stdout.write(".")
		sys.stdout.flush()
	print "OK"
	print "Generando input...",
	sys.stdout.flush()
	generar_input( 'test_no_libre.in', instancias )
	print "OK"
	#generar_densidad(100,0.3,10);

	#generar_densidad(100,0.6,10);
	#generar_densidad(100,0.9,10);

if __name__=="__main__":
	main()
