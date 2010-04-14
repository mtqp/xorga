#!/usr/bin/python
# Generador de archivos de test random para el ejercicio 1
from random import seed, randint

def main():
	seed()

	cantidad = 100
	maximo_b = 200
	maximo_n = 1000

	fIn  = open('test.in', 'w')
	fOut = open('test.out','w')

	for i in range(cantidad):
		b = randint(0,maximo_b)
		n = randint(1,maximo_n)
		r = (b**n)%n
		fIn.write('%s %s\n'%(b,n))
		fOut.write('%s\n'%r)

	fIn.write('-1 -1')
	fIn.close()
	fOut.close()

if __name__=="__main__":
	main()
