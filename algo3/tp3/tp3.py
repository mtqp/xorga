import random

def grafoSinAristas( n ):
	return [ [] for i in range(n) ]

def agregarClique( grafo, nodos ):
	for nodo in nodos:
		for v in nodos:
			if v != nodo and nodo not in grafo[v-1]:
				grafo[v-1].append( nodo )

def agregarRandom( grafo ):
	n = len(grafo)
	nodos = [ x+1 for x in range(n) ]
	for i in range(n):
		posibles  = list(set(nodos)-set(grafo[i]))
		elegidos  = random.sample( posibles , random.randint(0,len(posibles) ) )
		grafo[i] += elegidos

def grafoCliqueMin( n, n_clique ):
	g = grafoSinAristas( n )
	nodos_clique = random.sample([ x+1 for x in range(n) ],n_clique)
	agregarClique( g, nodos_clique )
	agregarRandom( g )
	return g


def guardarInput(nombre, instancias):
	fIn = open(nombre,'w')
	for instancia in instancias:
		fIn.write("%d\n"%len(instancia))
		for nodo in instancia:
			fIn.write("%d "%len(nodo))
			for v in nodo:
			 	fIn.write("%d "%(v))
	 		fIn.write("\n")
	fIn.write("-1")
	fIn.close()

def cargarInput( archivo ):
	grafos = []
	f = open(archivo,'r')
	linea = f.readline().strip()
	while( linea != "-1" ):
		grafo = []
		n = int(linea)
		for nodo in range(n):
			linea = f.readline().strip()
			ejes  = linea.split()[1:]
			adyacentes = [ int(x) for x in ejes ]
			grafo.append( adyacentes )
		grafos.append(grafo)
		linea = f.readline().strip()
	return grafos

def cargarOutput( archivo ):
	subgrafos = []
	f = open(archivo,'r')
	for linea in f:
		s = linea.strip()
		if s[0] == 'N':
			nodos    = s[1:].strip().split()
			subgrafo = [ int(x) for x in nodos ]
			subgrafos.append( subgrafo )
	return subgrafos
	
def cargarCounts( archivo ):
	counts = []
	nro_linea = 0
	f = open(archivo,'r')
	for linea in f:
		s = linea.strip()
		n,count = s.split()
		counts.append( (nro_linea,n,count) )
		nro_linea += 1
	return counts

def grafo_random( n ):
	grafo = [ [] for i in range(n) ]
	for i in range(n):
		grafo[i] += [ x+1 for x in range(i)     if i+1 in grafo[x] ]
		grafo[i] += [ x+1 for x in range(i+1,n) if random.randint(0,1) == 1 ]
	return grafo

def graphviz( archivo, grafos, soluciones=[] ):
	f = open(archivo,'w')
	for i in range(len(grafos)):
		grafo = grafos[i]
		solucion = soluciones[i] if len(soluciones) > 0 else []
		f.write( "graph G { \n" )
		n = len(grafo)
		f.write( "node[shape=circle]" )
		for nodo in range(1,n+1):
			if nodo not in solucion:
				f.write("%d;\n"%(nodo))
		f.write( "subgraph max_clique {\n" )
		f.write( "node[color=red];\n" )
		f.write( "edge[color=red];\n" )
		for nodo in range(1,n+1):
			if nodo in solucion:
				f.write("%d;\n"%(nodo))
				for adyacente in grafo[nodo-1]:
					if adyacente in solucion and adyacente > nodo:
						f.write("%d -- %d;\n"%(nodo,adyacente))
		f.write( "}\n" )
	
		for nodo,adyacentes in enumerate(grafo):
			for adyacente in adyacentes:
				if nodo+1 not in solucion and (adyacente > nodo+1 or adyacente in solucion):
					f.write("%d -- %d;\n"%(nodo+1,adyacente))
		f.write( "}\n" )

def lineaInstancia( instancias, numero_instancia ):
	linea = 0
	for i in range(numero_instancia):
		linea += len( instancias[i] )
	return linea + numero_instancia +1