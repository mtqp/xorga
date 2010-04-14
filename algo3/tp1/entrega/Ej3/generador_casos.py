#!/usr/bin/python

from random import randint, seed

casos = 1000
max_programadores = 100

seed()

fIn = open('test.in','w')

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
