#  !/usr/bin/python

from random import randint, seed

programadores = 1000

seed()

fIn = open('test_ing_eg.in','w')

ingresos = []
egresos = []
horas = 0
minutos = 0
segundos = 0
for i in range(1,programadores):
	ingresos += ["%.2d:%.2d:%.2d %s"%(horas,minutos,segundos,i+1)]
        segundos += 1
        if segundos == 60:
        	segundos = 0
        	minutos += 1
        	if minutos == 60:
        	        minutos == 0
        	        horas += 1
    	egresos += ["%.2d:%.2d:%.2d %s"%(horas,minutos,segundos,i)]
        segundos += 1
        if segundos==60:
        	segundos = 0
        	minutos +=1
        	if minutos == 60:
        	        minutos =0
        	        horas += 1
	
        fIn.write("%s\n"%(i))
        for p in range(i):
        	fIn.write("%s\n"%(ingresos[p]))

       	for p in range(i):
       		fIn.write("%s\n"%(egresos[p]))
fIn.write("-1")

