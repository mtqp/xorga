#!/usr/bin/python
# Generador de archivos de test random para el ejercicio 1

from optparse import OptionParser

from random import seed, randint
import sys

def main(options):
	seed()

	cantidad = options.count
	maximo_b = options.b
	maximo_n = options.n
	nombre   = options.archivo

	fIn  = open(nombre+'.in' ,'w')

	if options.out:	
		fOut = open(nombre+'.out','w')

	for i in range(cantidad):
		b = randint(0,maximo_b)
		n = randint(1,maximo_n)
		fIn.write('%s %s\n'%(b,n))
		if options.out: 
			r = (b**n)%n
			fOut.write('%s\n'%r)

	fIn.write('-1 -1')
	fIn.close()
	if options.out: 
		fOut.close()

if __name__=="__main__":
	parser = OptionParser()
	parser.add_option("-a", "--archivo", dest="archivo", help="escribe los test en ARCHIVO.in [ARCHIVO.out]", metavar='ARCHIVO', default='test')
	parser.add_option("-o", "--out", dest="out", help="escribe el resultado del test", action='store_true', default=False)
	parser.add_option("-c", "--count", dest="count", help="genera COUNT casos de prueba", type='int', default=1000, metavar='COUNT')
	parser.add_option("-b", "--max_b", dest="b", help="establece el rango de b desde 0 hasta B", type='int', default=200, metavar='B')
	parser.add_option("-n", "--max_n", dest="n", help="establece el rango de n desde 1 hasta N", type='int', default=100000, metavar='N')
	(options, args) = parser.parse_args()
	main(options)
