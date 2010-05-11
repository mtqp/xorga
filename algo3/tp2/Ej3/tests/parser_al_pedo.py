#!/usr/bin/python

def graphviz( nombre, mapa ):
	f = open(nombre,'w')
	f.write( "graph G { \n" )
	n,puertas,pasillos = mapa
	for i in range(n):
		f.write("%d;\n"%(i+1))
	for pasillo in pasillos:
		i,j = pasillo
		f.write("%d -- %d;\n"%(i,j))
	f.write( "}" )


if __name__=="__main__":
	f = open("archivo.in",'r')
	l = f.readline()
	l = l.split(" ")
	n = int(l[0])
	cl = int(l[1])
	cp = int(l[2])
	for i in range(cl):
		l = f.readline()
	pasillos = []
	for i in range(cp):
		l = f.readline().strip()
		l = l.split(" ")
		print l
		pasillos.append( ( int(l[0]), int(l[1]) ) )
	graphviz("archivo.dot", (n,[],pasillos) )

