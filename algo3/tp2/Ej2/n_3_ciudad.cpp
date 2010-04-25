#include<cmath>
#include<iostream>
#include<queue>
#include "../medicion.h"
#include "n_3_ciudad.h"
#define no_fuertemente_conexo 	false
#define fuertemente_conexo		true 

using namespace std;

///////////////////////AUXILIARES////////////////////////////
void borrar_nodo_lista(Lista *aristas, Nodo_lista* ptr_arista){
	bool unico_elem  = false;
	bool primer_elem = false;
	bool ultimo_elem = false;
	
	Nodo_lista *anterior;
	Nodo_lista *siguiente;
	
	if((ptr_arista == aristas->_primero_lista) && (ptr_arista == aristas->_ultimo_lista){
		aristas->_primero_lista = NULL;
		aristas->_ultimo_lista  = NULL;
		unico_elem = true;
	}
	
	if (ptr_arista == aristas->_primero_lista){
		siguiente = ptr_arista->_siguiente_lista;
		aristas->_primero_lista = siguiente;
		primer_elem = true;
	}
	
	if (ptr_arista == aristas->_ultimo_lista){
		anterior = ptr_arista->_anterior_lista;
		aristas->_ultimo_lista = anterior;
		ultimo_elem = true;
	}
	
	if ((!unico_elem)&&(!primer_elem)&&(!ultimo_elem)){		
			anterior 	= ptr_arista->_anterior_lista;
			siguiente	= ptr_arista->_siguiente_lista;	
	
			anterior->_siguiente_lista = siguiente;
			siguiente->_anterior_lista = anterior;
	
			if (anterior == aristas->_primero_lista){
				aristas->_primero_lista = anterior;	
			}
			if (siguiente == aristas->_ultimo_lista){
				aristas->_ultimo_lista = siguiente;
			}
	}
	delete[] ptr_arista;
}

bool una_arista_dos_nodos(Nodo_matriz** matriz_adyacencia, uint n){
	//si devuelve true ==> no es FC
	/**costo de esta funcion, O(n²), desde la funcion ciudad se llama UNA sola vez**/
	int res = 0;
	for (int i=0;i<n;i++)
		for(int j=0;j<n;j++)	//el for este inicializa j en i+1!!!
			res += matriz_adyacencia[i][j]._adyacente;
	return (res == 2);		//puede ser igual a uno, repensar xq la matriz es triangularsdojadfg
}

//arreglar tbm como lo de arriba!
bool algun_vertice_suelto(Nodo_matriz** matriz_adyacencia, uint n){
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

void setear_array_false(bool* nodos_marcados, uint n){
	//costo == O(n) (se realiza una sola vez)
	for (int i=0;i<n;i++)
		nodos_marcados[i] = false;
}

Nodo_lista *dame_nodo_camino_no_visto(Lista *aristas){
	return aristas->_primero_lista;
}

bool tengaAdyacente_y_noEsXDondeVenia(Nodo_matriz** matrix, uint tam_matrix, Lista *aristas, Queue<Nodo_lista*> &q, uint * nodos_marcados, Nodo_lista* nodo_camino, Nodo_lista* sig_adyacente))	
	/*	un camino viable (usara q no sea un nodo marcado para q no loopee, y los posible caminos q le restan
		si no encuentra, devuelve falso
		si encuentra, devuelve true, y guarda el numero de nodo en sig_adyacente

		tengo q buscar en la matrix[nodocamino->pi2] los bools en UNO tal q ptr!=NULL y no estan en Q ya (**), y si lo 
		encuentro guardarlo en sig_adyacente  
		
		(**) 	
	*/
	
	Nodo_lista* primer_q = q.front();
	
	int i = nodo_camino->_pi2;
	int j = 0;
	while(j<tam_matrix){
		if((matrix[i][j])->_adyacencia && ((matrix[i][j])->_ptr_arista != NULL)){
			if(no_esta_en_queue(q, nodo_camino)){
				FIJATE Q ESTE EN NODO CAMINO LOCOOOO
				return true;
			}
		}
	
	}
	return false;


}

/////////////////////////////////////////////////////////////
//////////////////////////FUNCION////////////////////////////
/////////////////////////////////////////////////////////////

/*termino siendo DFS.*/

bool ciudad(Nodo_Matriz** matriz_adyacencia, Lista *aristas_sin_camino, uint tam_matriz){
	/*
	necesito:
		- la matriz de adyacencia/ptr a la lista
		- una lista de arista, q una arista este en la lista ==> la arista no forma parte de un camino 
		simple todavia.
		- un array de bools de los nodos, para cuando se busque cada camino se chequee q no estes loopeando.
	*/

	if(una_arista_dos_nodos(matriz_adyacencia, tam_matriz)) = return no_fuertemente_conexo;
	if(algun vertice suelto(matriz_adyacencia, tam_matriz)) = return no_fuertemente_conexo;
	
	/*-------------------------------------------------------------
							si no es un caso base ---->
	------------------------------------------------------------*/
	
	bool nodos_marcados[n];
	setear_array_false(nodos_marcados,n);
	
	queue<Nodo_lista*> q; 								//estr FIFO

	bool con_caminos;
	bool es_camino_simple;

	Nodo_lista* primer_push;
	Nodo_lista* nodo_camino;
	Nodo_lista* sig_adyacente;

	while(aristas_sin_camino->_primero_lista != NULL){			//==> existen todavia aristas sin caminos
																// costo del while == O(n²)
		//q empty
		primer_push = dame_nodo_camino_no_visto(aristas_sin_camino);
		nodo_camino = primer_push;
		q.push(primer_push);
		
		nodos_marcados[nodo_camino->_pi1] = true;
	
		bool debe_seguir_armando;
		while(con_caminos = tengaAdyacente_y_noEsXDondeVenia(matriz_adyacencia, tam_matriz, aristas_sin_camino, q, nodos_marcados, nodo_camino, sig_adyacente, &debe_seguir_armando)){
					
			if (!debe_seguir_armando)	//listo busco siguentes caminos
				break;
		
			nodos_marcados[sig_adyacente->_pi1] = true;					//sino, sigo encolando el posible camino
			q.push(sig_adyacente);
		}

		if (!con_caminos) return no_fuertemente_conexo;	//ya esta, alguno no puede lograr camino
		if (!debe_seguir_armando){
			while(!q.empty()){					//vacio la estr FIFO
				Nodo_lista* popeado = q.front();
				q.pop();
				borrar_nodo_lista(aristas_sin_camino, (matriz_adyacencia[popeado->_pi1][popeado->_pi2])->_ptr_arista);
				(matriz_adyacencia[popeado->_pi1][popeado->_pi2])->_ptr_arista = NULL;
				nodo_marcado[popeado->_pi1] = false;	//reseteo el marcados para volver a buscar caminos
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

/*

ESTE MAIN TIENE Q CREAR ESA MATRIZ <BOOL, PTR_NODO>
Y LA LISTA DE NODOS!!!!!!!11

*/


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
 
