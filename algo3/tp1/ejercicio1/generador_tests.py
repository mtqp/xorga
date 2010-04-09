# Generador de archivos de test random para el ejercicio 1

import random

random.seed()

cantidad = 500
maximo_b = 200
maximo_n = 10000

fIn  = open('test.in', 'w')
fOut = open('test.out','w')

for i in range(cantidad):
	b = random.randint(0,maximo_b)
	n = random.randint(1,maximo_n)
	r = (b**n)%n
	fIn.write('%s %s\n'%(b,n))
	fOut.write('%s\n'%r)

fIn.write('-1 -1')
fIn.close()
fOut.close()

