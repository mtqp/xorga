#!/usr/bin/python
# Generador de casos random para el ejercicio 3

from optparse import OptionParser
from random import randint, seed

def main(options):
	casos = options.count
	max_programadores = options.n

	seed()

	fIn = open(options.archivo+'.in','w')

	ingresos = []
	egresos = []

	for i in range(casos):
		programadores = randint(0,max_programadores-1)
		caso = randint(0,1)
		for j in range(programadores-caso):
			horas = randint(0,23)
			minutos = randint(0,58)
			segundos = randint(0,58)
			ingresos += ["%.2d:%.2d:%.2d %s"%(horas,minutos,segundos,j+1)]
		
			horas_e = randint(horas,23)
			if horas_e == horas:
				minutos_e = randint(minutos,59)
			else:
				minutos_e = randint(0,59)
			if minutos_e == minutos:
				segundos_e = randint(segundos,59)
			else:
				segundos_e = randint(0,59)
			egresos += ["%.2d:%.2d:%.2d %s"%(horas_e,minutos_e,segundos_e,j+1)]

		if caso == 1:
			ingresos += ["23:59:58 %s"%(programadores)]
			egresos += ["23:59:59 %s"%(programadores)]
		
		ingresos=sorted(ingresos)
		egresos=sorted(egresos)
	
		fIn.write("%s\n"%(programadores))
		for j in range(programadores):
			fIn.write("%s\n"%(ingresos[j]))
	
		for j in range(programadores):
			fIn.write("%s\n"%(egresos[j]))
		ingresos = []
		egresos = []
	fIn.write("-1")

if __name__=="__main__":
	parser = OptionParser()
	parser.add_option("-a", "--archivo", dest="archivo", help="escribe los test en ARCHIVO.in [ARCHIVO.out]", metavar='ARCHIVO', default='test')
	parser.add_option("-c", "--casos", dest="count", help="genera COUNT casos de prueba", type='int', default=1000, metavar='COUNT')
	parser.add_option("-n", "--programadores", dest="n", help="establece la cantidad maxima de programadores por caso", type='int', default=100, metavar='N')
	(options, args) = parser.parse_args()
	main(options)

