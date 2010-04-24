#include<cmath>
#include<iostream>
#include<stack>
#include "../medicion.h"
#define pila stack<uint>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

/////////////////////////////////////////////////////////////
//////////////////////PSEUDO-CODE////////////////////////////
/////////////////////////////////////////////////////////////


/*termino siendo DFS creo.*/

bool ciudad(uint** matriz_adyacencia, uint tam_matriz){
	/*
	necesito:
		- la matriz de adyacencia
		- la copia de la matriz de adyacencia (booleana) pero en vez de decir si hay una arista entre (u,v), 
			dice si esa arista ya forma parte de un camino simple.
		- un array de bools de los nodos, para cuando se busque cada camino se chequee q no estes loopeando.
	*/

	case: [una arista/dos nodos] = return no_fuertemente_conexo
	case: [algun vertice suelto] = return no_fuertemente_conexo
	-------------------------------------------------------------
	sino ---->
	------------------------------------------------------------
	bool matriz_aristas_caminos [tam_matriz][tam_matriz];
	setear_false_toda(matriz_aristas_caminos, tam_matriz); 
	
	bool nodos_marcados[n];
	setear_false_todo(nodos_marcados,n);
	
	cola c; //(no se si la cola era, tiene q ser FIFO)
	int M = cantidad_aristas(matriz_adyacencia, tam_matriz)/2;

	while(M>0){
		//siempre entra aca con una estr FIFO vacia (si se llama cola mejor jajaj)
		int primer_push = dame_nodo_camino_no_visto(DE_ALGUN_LADO);
		c.push(primer_push);
		bool con_caminos = true;
	
		int nodo_camino = primer_push;
		nodos_marcados[nodo_camino] = true;
	
		bool es_camino_simple;
	
		while(con_caminos = tengaAlgunAdyacente_y_noEsXDondeVenia(nodo_camino,nodos_marcados,int sig_adyacente)){
					
			if (es_camino_simple = (sig_adyacente == primer_push))	//listo busco siguentes caminos
				break;
		
			nodos_marcados[sig_adyacente] = true;					//sino, sigo encolando el posible camino
			c.push(sig_adyacente);
		}

		if (!con_caminos) return no_fuertemente_conexo	//ya esta, alguno no puede lograr camino
		else {
			if(es_camino_simple){
				while(!c.empty()){					//vacio la estr FIFO
					uint popeado = c.front();
					c.pop();
					matriz_aristas_caminos[popeado][c.front()] = true;
					matriz_aristas_caminos[c.front()][popeado] = true;
					nodo_marcado[popeado] = false;	//reseteo el marcados para volver a buscar caminos
					M--;							//decremento M = M-aristasDeLCamino
				}
			}
		}
	}
	//si sale aca es xq paso x todas las aristas y siempre encontro camino para volver
	return es_fuertemente_conexo;
}
////////////////////////////////////////////

int cantidad_aristas(matriz M, int n){
	int res = 0;
	for (i in range n)
		for(j i range n)
			res += M[i][j];
}

dame_nodo_camino_no_visto(DE_ALGUN_LADO){
	/*
	estaba pensando q podria tener una lista de los vertices q use q irlos borrando
	a medida q encuentro camino, entonces dame_nodo_camino_no_visto, te tira el 1er camino de la lista
	y de ahi sacas el nodo para empezar
	*/
	
}

bool tengaAlgunAdyacente_y_noEsXDondeVenia(nodo_camino,(array de) nodos_marcados, int sig_adyacente){
	/*
		es descriptivo el nombre, jajaja, es eso, q busque en los posible adyacentes alguno q sea 
		un camino viable (usara q no sea un nodo marcado para q no loopee, y los posible caminos q le restan
		si no encuentra, devuelve falso
		si encuentra, devuelve true, y guarda el numero de nodo en sig_adyacente
	*/
}

//tengo la sensacion q es n², de todas formas chequeenlo antes q me siente a escribirlo bien
//hay casos q no contemplo? cualquier ayuda modificacion es bienvenida, buen finde!

/////////////////////////////////////////////////////////////
////////////////////FIN-PSEUDO-CODE//////////////////////////
/////////////////////////////////////////////////////////////


void print_matriz(uint** M, uint m, uint dos){
	for(uint i=0; i<m; i++){
		for(uint j=0; j<dos; j++) cout << M[i][j];
		cout << endl;
	}
}

void print_vector(uint* V, uint n){
	for(uint i=0; i<n; i++) cout << V[i];
	cout << endl;
}

int main (int argc, char** argv){
	ullint n;
	double ts;
	uint cantidad_de_calles;
	uint esquina;
	while(cin >> n && n!=(ullint)-1){
		uint** conexiones;
		conexiones = new uint* [n];
		for(uint i=0; i<n; i++){
			conexiones[i] = new uint [n];
			for(uint j=0; j<n; j++) conexiones[i][j]=0;
		}
		for(uint i=0; i<n; i++){
			cin >> cantidad_de_calles;
			for(uint j=0; j<cantidad_de_calles; j++){
				cin >> esquina;
				conexiones[i][esquina-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, ciudad(conexiones,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			ciudad(conexiones,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			if(ciudad(conexiones,n)) cout << "fuertemente conexo" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0; i<n; i++){	//libero la memoria
			delete [] conexiones[i];
		}
		delete [] conexiones;
	}
	return 0;
}
 
