#!/usr/bin/python
import math

def ciclo_rando(n,ciclo): #ciclo rando del orto... me voy a dormir, son las 6 49 am... y "casi compila" (valga compilar para python)
	#ciclo = [ [] for i in range(n) ]

    for i in range(1,n):
        if i-1 == 0:
            ciclo[0] += [(0,m)]
        if i-1 == m:
            ciclo[i-1] += [(0,i-1)]
        else:
            ciclo[i-1] += [i-1,i]
            ciclo[i-1] += [ k for k in range(i+1,n+1) if k not in ciclo[i-1] and randint(1,4)==1 ]
	#for i in range(1,n+1):
	#	ciclo[i-1] = [ k for k in range(1,n+1) if k in ciclo[i-1] or i in ciclo[k-1] ]
	#for i in range(1,n+1):
	#	if i-1 not in ciclo[i-1] and i!=1:
	#		ciclo[i-1].append(i-1)
	#	if i+1 not in ciclo[i-1] and i!=n:
	#		ciclo[i-1].append(i+1)
	#	ciclo[i-1].sort()
	#if n not in ciclo[0]:
	#	ciclo[0].append(n)
    #if 1 not in ciclo[n-1]:
	#	ciclo[n-1].append(1)
	return ciclo


from random import randint, seed, choice

def agregar_hijos(padre,arbol,desde):
    arbol.append([padre + desde , desde + 2*padre + 1])
    arbol.append([padre + desde , desde + 2*padre + 2])

def crear_arbol_binario(tam, desde,arbol):
    for i in range(tam/2):
        agregar_hijos(i,arbol,desde)

def berni_cago_fuego(cant_llaves, tam,llaves,habitaciones):
    print habitaciones
    if habitaciones  % 2 == 0:
        if habitaciones/2 % 2 == 0:
            print "if if"
            puerta = habitaciones/4
            llave = 2*puerta + 1
        else:
            print "if else"
            puerta = (-1 + habitaciones/2)/2
            llave = 2*puerta + 1
    else:
        if (habitaciones -1)/2 % 2 == 0:
            print "else if"
            puerta = (habitaciones-1)/4
            llave = 2*puerta + 1
        else:
            print "else else"
            puerta = ((-1 +habitaciones) /2 -1) /2
            llave = 2*puerta + 1
    print llave
    print puerta
    llaves.append([llave,puerta])
    for i in range(tam/2 - 1):
        condicional = randint(1,2)
        if condicional == 2:
            puerta =randint(2,-2 +tam/2)
            llave = 2*puerta + 1
            if llaves.count([llave,puerta]) == 0:
                llaves.append([llave,puerta])


def main(n): #diegoooo, estas re escabio... si, con B!!werwerwerwker?lwktlw
    casos = 10
    max_habitaciones = 10

    seed()

    f = open('test.in','w')
    #for i in range(casos):
    habitaciones = n
    #randint(4,max_habitaciones)
    pasillos = habitaciones - 1
    cant_llaves = 5
    arbol = []
    llaves = []
    m = 10
    ciclo = [ [] for i in range(m) ]#che... prende la luz que no ves nada!
    ciclo_rando(m,ciclo)
    print "ciclo : "
    print ciclo
    crear_arbol_binario(habitaciones - m,1,arbol)
    print arbol
    berni_cago_fuego(cant_llaves,habitaciones,llaves,habitaciones) #le mande 2 veces habitaciones para hacer mas rapido un debug... pifie mio :p
    print llaves
    f.write("%d %d %d\n"%(habitaciones, cant_llaves, pasillos))
    print llaves
    print arbol
    for llave in llaves:
        f.write("%d %d\n"%(llave[0],llave[1]))
#hablando en serio... (si es que va separado)... falta escribir ciclo en el archivo
    for nodo in arbol:
        f.write("%d %d\n"%(nodo[0],nodo[1]))
    f.write("-1 -1 -1")
    f.close()




