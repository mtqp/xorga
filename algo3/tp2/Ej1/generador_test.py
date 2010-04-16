#!/usr/bin/python
# Generador de archivos de test random para el ejercicio 1
from random import seed, randint

def main():
	seed()

	cantidad = 15
	maximo_n = 20
	maximo_elem = 20
	
	fIn  = open('test.in', 'w')

	for i in range(cantidad):
		n = randint(1,maximo_n)
		fIn.write('%s   '%(n))
		for j in range(n):
			elem = randint(-maximo_elem,maximo_elem)
			fIn.write('%s '%(elem))
		fIn.write('\n')

	fIn.write('-1')
	fIn.close()

if __name__=="__main__":
	main()
