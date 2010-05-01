#!/usr/bin/python
# Generador de archivos de test para el ejercicio 1
from random import seed, randint

def generar_random(cantidad, maximo_b, maximo_n):
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


def generar_no_if(cantidad, maximo_b, maximo_log_n):
	fIn  = open('test_no_if.in', 'w')

	for i in range(cantidad):
		b = randint(0,maximo_b)
		n = 2**randint(1,maximo_log_n)
		fIn.write('%s %s\n'%(b,n))

	fIn.write('-1 -1')
	fIn.close()
	

def sucesion_if(i):
	#n_i = 2*n_(i-1)+1
	if i == 1:
		return 1
	return 2*sucesion_if(i-1)+1

def generar_if(cantidad, maximo_b, maximo_n):
	fIn  = open('test_if.in', 'w')

	for i in range(cantidad):
		b = randint(0,maximo_b)
		n = sucesion_if(randint(1,maximo_n))
		fIn.write('%s %s\n'%(b,n))

	fIn.write('-1 -1')
	fIn.close()


def main():
	seed()

	cantidad = 100
	maximo_b = 200
	
	generar_random(cantidad, maximo_b, 1000);
	generar_no_if(cantidad, maximo_b, 30);
	generar_if(cantidad, maximo_b, 30);


if __name__=="__main__":
	main()
