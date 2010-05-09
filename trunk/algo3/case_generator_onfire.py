#!/usr/bin/python
import math

from random import randint, seed, choice

def ciclo_rando(n):
    ciclo = [ [] for i in range(n-1) ]
    for i in range(n):
        if (i!=0):
            if (i==1):
                ciclo[0] += [(1,n)]
            ciclo[i-1] += [(i,i+1)]
            ciclo[i-1] += [(i,k) for k in range(i+2,n+1) if k not in ciclo[i-1] and randint(1,4)==1 ]
    return ciclo


def agregar_hijos(padre,arbol,desde):
    arbol.append([padre + desde , desde + 2*padre + 1])
    arbol.append([padre + desde , desde + 2*padre + 2])

def crear_arbol_binario(tam, desde,arbol):
    for i in range(tam/2):
        agregar_hijos(i,arbol,desde)

def berni_cago_fuego(llaves,habitaciones,desde):
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
    llaves.append([llave + desde,puerta + desde])
    for i in range(habitaciones/2 - 1):
        condicional = randint(1,2)
        if condicional == 2:
            puerta =randint(2,-2 +habitaciones/2)
            llave = 2*puerta + 1
            if llaves.count([llave + desde,puerta + desde]) == 0:
                llaves.append([llave + desde,puerta + desde])


def main(n):
    casos = 10
    max_habitaciones = 10

    seed()
    f = open('test_fire.in','w')
    #for i in range(casos):
    habitaciones = n
    #randint(4,max_habitaciones)
    nodos_a = 10

    arbol = []
    llaves = []
    ciclo = ciclo_rando(habitaciones-10)
    desde  = randint(2,habitaciones-nodos_a)
    print desde
    crear_arbol_binario(nodos_a,desde,arbol)
    berni_cago_fuego(llaves,nodos_a,desde) #le mande 2 veces habitaciones para hacer mas rapido un debug... pifie mio :p
    pasillos = len(arbol)
    habitaciones = len(arbol) + len(ciclo) + 1
    for i in ciclo:
        pasillos += len(i)
    f.write("%d %d %d\n"%(habitaciones, len(llaves), pasillos))
    print llaves
    print arbol
    print ciclo
    print habitaciones
    print len(llaves)
    print pasillos
    for llave in llaves:
        f.write("   %d %d\n"%(llave[0],llave[1]))

    for nodo in arbol:
        f.write("      %d %d\n"%(nodo[0],nodo[1]))
    for j in ciclo:
        for i in j:
            f.write("      %d %d\n"%(i[0],i[1]))
    f.write("-1 -1 -1")
    f.close()





