#!/usr/bin/python

from random import randint
import sys

def secuencia_random( n ):
	return [ randint(-50,50) for i in range(n) ]

def secuencia_creciente( n, s0=False ):
	if n == 0: return []
	if s0 == False:
		s = [ randint(-50,50) ]
	else:
		s = [ s0 ]
	for i in range(n-1):
		s.append(s[i]+randint(1,20))
	return s

def secuencia_decreciente( n, s0=False ):
	if n == 0: return []
	if s0 == False:
		s = [ randint(-50,50) ]
	else:
		s = [ s0 ]
	for i in range(n-1):
		s.append(s[i]-randint(1,10))
	return s

def secuencia_unimodal( n ):
	s  = secuencia_creciente(randint(1,n))
	s += secuencia_decreciente( n-len(s), s[len(s)-1]-(-1 if s[len(s)<0] else 1)*randint(1,20) )
	return s

def generar_input( nombre,  instancias ):
	f = open(nombre,'w')
	for i in instancias:
		s = reduce( lambda x,y: "%s %s"%(x,y), i )
		f.write("%d %s\n"%(len(i), s))
	f.write("-1")

def generar_instancias( n, generador=secuencia_random ):
	instancias = []
	for i in range(1,n+1):
		instancias.append( generador( i ) )
	return instancias

if __name__ == "__main__":
	n = 1000
	print "Cantidad de instancias: %d"%(n)
	print "Generando instancias de secuencia unimodal..",
	sys.stdout.flush()
	instancias = generar_instancias( n, secuencia_unimodal )
	generar_input( "test_unimodal.in", instancias )
	print "OK"
	print "Generando instancias de secuencia random..",
	sys.stdout.flush()
	instancias = generar_instancias( n, secuencia_random )
	generar_input( "test_random.in", instancias )
	print "OK"
	print "Generando instancias de secuencia creciente..",
	sys.stdout.flush()
	instancias = generar_instancias( n, secuencia_creciente )
	generar_input( "test_creciente.in", instancias )
	print "OK"
	print "Generando instancias de secuencia decreciente..",
	sys.stdout.flush()
	instancias = generar_instancias( n, secuencia_decreciente )
	generar_input( "test_decreciente.in", instancias )
	print "OK"