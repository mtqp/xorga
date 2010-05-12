#!/usr/bin/python
#
# llaves = [...]
# pasillos = [...]
#

def peor_caso(n):

	llaves = []
	pasillos = []

	#defino los pasillos que conectan con el nodo inicial
	for i in range( 2, (n-2)/2+3 ):
		pasillos.append( (1,i) )
	
	#defino los pasillos entre los nodos del primer y segundo nivel
	for i in range( 2, (n-2)/2+2+(n%2) ):
		pasillos.append( (i, n-i+2) )
	
	for i in range( 2, (n-2)/2+2 ):
		llaves.append( (n-i+1, i) )

	return (n,llaves,pasillos)


def generar_input( nombre, instancias ):
	fIn = open(nombre,'w')
	for instancia in instancias:
		n,puertas,pasillos = instancia
		fIn.write( "%d %d %d\n"%(n,len(puertas),len(pasillos)) )
		for puerta in puertas:
			esta, abre = puerta
			fIn.write( "  %d %d\n"%(esta, abre) )
		for pasillo in pasillos:
			extremo1, extremo2 = pasillo
			fIn.write( "    %d %d\n"%(extremo1, extremo2) )
	fIn.write("-1 -1 -1")
	fIn.close()	


if __name__=="__main__":
	instancias = []
	for n in range(4,101):
		instancias.append( peor_caso(n) )
	
	generar_input( "peor_caso.in", instancias )


