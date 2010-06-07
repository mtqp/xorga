def cargarInput( archivo ):
	grafos = []
	f = open(archivo,'r')
	linea = f.readline().strip()
	while( linea != "-1" ):
		grafo = []
		n = int(linea)
		for nodo in range(n):
			linea = f.readline().strip()
			ejes  = linea[1:].strip().split()
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
	

def graphviz( archivo, grafos, soluciones ):
	f = open(archivo,'w')
	for i in range(len(grafos)):
		grafo = grafos[i]
		solucion = soluciones[i]
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
