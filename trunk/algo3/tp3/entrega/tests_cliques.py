#!/usr/bin/python
from tp3 import graphviz
import random, sys

def todos_con_todos(n):
        adyacencias = [ [ x for x in range(1,n+1) if x!=i  ] for i in range(1,n+1) ]
	return adyacencias

def unir_ciclos(ciclo, ciclo2, fc=True):
        max = len(ciclo)
        desde = random.randint(0,max-1)
        hasta = random.randint(0,max-1)
        ciclo[desde] += [max+1]
        if fc: ciclo[hasta] += [max+len(ciclo2)]
        ciclo += [ [ i+max  for i in x ] for x in ciclo2  ]
        ciclo[max] += [desde+1]
        if fc: ciclo[max+len(ciclo2)-1] += [hasta+1]
        return ciclo

def generar_input(nombre, ciclos):
        f = open(nombre,'w')
        for ciclo in ciclos:
                f.write( '%d\n'%( len(ciclo) ))
                if len(ciclo) > 0:
                        for i in range(len(ciclo)):
                                if len(ciclo[i]) > 0:
                                        s = reduce( lambda x,y: "%s %s"%(x,y), ciclo[i] )
                                else:
                                        s = ""
                                f.write('%d %s\n'%( len(ciclo[i]), s) )
        f.write("-1")
        f.close()

def generar_instancias( instancias, fc=True ):
        generadores = [ todos_con_todos ]
        ciclos = []
        for i in instancias:
                instancia = []
                for c in i:
                        if type(c)==type(tuple()):
                                n,gen = c
                        else:
                                n = c
                                gen = random.choice(generadores)
                        ciclo = gen(n)
                        if len(instancia)>0:
                                unir_ciclos(instancia,ciclo,fc)
                        else:
                                instancia=ciclo
                ciclos+=[instancia]
        return ciclos

if __name__=="__main__":
        #ciclo = unir_ciclos(ciclo_minimo(3),ciclo_minimo(4))
        #print unir_ciclos(ciclo,todos_con_todos(5))
        instancias = []
        cantidad_instancias = 100
        print "Cantidad de instancias: %d"%(cantidad_instancias)
        print "Eligiendo instancias para generar",
        for n in range(15,15+cantidad_instancias):
                k = [ [a,b,c] for a in range(random.randint(3,n-10),n) for b in range(3,n) for c in range(3,n) if a+b+c == n ]
                s = random.sample(k,3)
                instancias += s
                sys.stdout.write( "." )
                sys.stdout.flush()
        print "OK"
        print "Generando instancias..",
        ciclos=generar_instancias( instancias )
        print "OK"
        print "Generando input...",
        generar_input( "test_cliques.in", ciclos )
        print "OK"

	graphviz( 'test_cliques.dot', ciclos )

        #instancias = [ [(x,mejor_caso)] for x in range(1,100) ]
        #ciclos=generar_instancias( instancias )
        #generar_input( "test_mejor_caso.in", ciclos )

