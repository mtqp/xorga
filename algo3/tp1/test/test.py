#!/usr/bin/env python

import getopt,sys
import subprocess
from random import randint,seed
from math import sqrt

def runtest(li,args):
	# abro ./amigos con parametros: time 0.1 3
	fp=subprocess.Popen(['./intervalos']+args, shell=False, stdin=subprocess.PIPE,stdout=subprocess.PIPE)
	for i in li:
		fp.stdin.write(str(len(i))+"\n"+' '.join(map(str,i))+"\n") # le manda la instancia
		r=fp.stdout.readline().rstrip() # recibo el resultado
		print "n=",len(i)," ",args[0],"=",r
		yield len(i),r # lo devuelvo
	fp.stdin.write("-1\n")

def usage():
	print "Usage:"
	print "-t (o --time) para calcular tiempos"
	print "-c (o --count) para contar operaciones"
	sys.exit(2)

try:
	opts, args = getopt.getopt(sys.argv[1:], "tc", ["time", "count"])
except getopt.GetoptError, err:
	# print help information and exit:
	print str(err) # will print something like "option -a not recognized"
	usage()
if not opts: usage()

opts=map(lambda x: x[0],opts)

seed(1234) # defino el seed para hacer el experimento reproducible

test=open('test.in')
li=[]
while 1:
	inst=map(int,test.readline().split())
	if inst[0]==-1: break
	li.append(inst[1:])

out=""
if "--time" in opts or "-t" in opts:
	for i in runtest(li,['time','0.1','3']): # para cada caso de prueba...
		vals=map(float,i[1].split()) # obtengo la lista de los tiempos
		val=reduce(lambda x,y:x+y,vals)/len(vals) # tomo el primedio
		err=max([abs(x-val) for x in vals]) # calculo el error
		dat = (str(i[0])+"\t"+ "%.8f"%val +"\t"+ "%.8f"%err +"\n") # lo preparo para el archivo de salida
		out += dat
	open('time_err.dat','w').write(out) # lo guardo en el archivo
	print "\nOutput escrito a time_err.dat"

out=""
if "--count" in opts or "-c" in opts:
	for i in runtest(li,['count']): #para cada caso de prueba..
		dat = str(i[0])+"\t"+i[1]+"\n" # lo formateo para la salida
		out += dat
	open('counts.dat','w').write(out) # lo guardo en el archivo
	print "\nOutput escrito a counts.dat"
