#include<cmath>
#include<iostream>
#include<queue>
#include "../medicion.h"
#define no_fuertemente_conexo 	false
#define fuertemente_conexo		true 

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

///////////////////////AUXILIARES////////////////////////////

bool una_arista_dos_nodos(uint** matriz_adyacencia, uint n){
	//si devuelve true ==> no es FC
	/**costo de esta funcion, O(n²), desde la funcion ciudad se llama UNA sola vez**/
	int res = 0;
	for (int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			res += matriz_adyacencia[i][j];
	return (res == 2);
}

bool algun_vertice_suelto(uint** matriz_adyacencia, uint n){
	//si devuelve true ==> no es FC
	/**costo de esta funcion, O(n²), desde la funcion ciudad se llama UNA sola vez**/
	int cant_adyacencias;
	for (int i=0;i<n;i++){
		cant_adyancencias = 0;
		for(int j=0;j<n;j++){
			cant_adyacencias += matriz_adyacencia[i][j];
		}
		if (cant_adyacencias == 0) return true;
	}
	return false;
}

void setear_matriz_false(bool** matriz_aristas_caminos, uint n, uint m){
	//costo == O(n²) (se realiza una sola vez)
	for (int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			matriz_aristas_caminos[i][j] = false;
}

void setear_array_false(bool* nodos_marcados, uint n){
	//costo == O(n) (se realiza una sola vez)
	for (int i=0;i<n;i++)
		nodos_marcados[i] = false;
}

int cantidad_aristas(bool** M, int n, int m){		//costo, O(n*m) = O(n²)
	int res = 0;
	for (int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			res += M[i][j];
	return res;
}

uint dame_nodo_camino_no_visto(DE_ALGUN_LADO){
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
//////////////////////////FUNCION////////////////////////////
/////////////////////////////////////////////////////////////

/*termino siendo DFS.*/

bool ciudad(uint** matriz_adyacencia, uint tam_matriz){
	/*
	necesito:
		- la matriz de adyacencia
		- la copia de la matriz de adyacencia (booleana) pero en vez de decir si hay una arista entre (u,v), 
			dice si esa arista ya forma parte de un camino simple.
		- un array de bools de los nodos, para cuando se busque cada camino se chequee q no estes loopeando.
	*/

	if(una_arista_dos_nodos(matriz_adyacencia, tam_matriz)) = return no_fuertemente_conexo;
	if(algun vertice suelto(matriz_adyacencia, tam_matriz)) = return no_fuertemente_conexo;
	
	/*-------------------------------------------------------------
							si no es un caso base ---->
	------------------------------------------------------------*/
	bool matriz_aristas_caminos [tam_matriz][tam_matriz];
	setear_matriz_false(matriz_aristas_caminos, tam_matriz, tam_matriz); 
	
	bool nodos_marcados[n];
	setear_array_false(nodos_marcados,n);
	
	queue<uint> q; 								//estr FIFO
	int M = cantidad_aristas(matriz_adyacencia, tam_matriz, tam_matriz)/2;

	bool con_caminos;
	bool es_camino_simple;
	uint nodo_camino;
	uint primer_push;
	uint sig_adyacente;

	while(M>0){
		//q empty
		primer_push = dame_nodo_camino_no_visto(DE_ALGUN_LADO);
		nodo_camino = primer_push;
		q.push(primer_push);
		
		nodos_marcados[nodo_camino] = true;
		//con_caminos = true;
	
		while(con_caminos = tengaAlgunAdyacente_y_noEsXDondeVenia(nodo_camino,ALGUN PARAM MAS CAPAS, nodos_marcados, &sig_adyacente)){
					
			if (es_camino_simple = (sig_adyacente == primer_push))	//listo busco siguentes caminos
				break;
		
			nodos_marcados[sig_adyacente] = true;					//sino, sigo encolando el posible camino
			q.push(sig_adyacente);
		}

		if (!con_caminos) return no_fuertemente_conexo;	//ya esta, alguno no puede lograr camino
		else {
			if(es_camino_simple){
				while(!q.empty()){					//vacio la estr FIFO
					uint popeado = q.front();
					q.pop();
					matriz_aristas_caminos[popeado][q.front()] = true;
					matriz_aristas_caminos[q.front()][popeado] = true;
					nodo_marcado[popeado] = false;	//reseteo el marcados para volver a buscar caminos
					M--;							//decremento M = M-aristasDeLCamino
				}
			}
		}
	}
	//si sale aca es xq paso x todas las aristas y siempre encontro camino para volver
	return fuertemente_conexo;
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
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
 
