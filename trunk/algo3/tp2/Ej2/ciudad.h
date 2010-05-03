#include<cmath>
#include<iostream>
#include<stack>
#include<queue>
#include<list>
#define pila stack<uint>
#define cola queue<uint>
#define lista list<uint>
#define no_fuertemente_conexo 	false
#define fuertemente_conexo		true 

typedef unsigned int uint;
typedef unsigned long long int ullint;

using namespace std;

bool ciudad(uint** conexiones, uint n);
bool dfs_busco_primer_ciclo(uint** conexiones, bool* nodos_ciclo, bool* nodos_ciclo_posible, uint n);
bool dame_adyacentes_a(uint nodo_actual,uint** conexiones, lista &l_ady, uint n);
bool dame_arista_libre_ciclo(uint &nodo_salida, uint** conexiones, bool* nodos_ciclo, uint n);
bool formar_ciclo_desde(uint& nodo_salida, uint** conexiones, bool* nodos_ciclos, uint& cant_nodos_ciclo, uint n);
uint nodos_en_ciclo(bool* nodos_ciclos, uint n);
void reseteo_matriz_salvo_ciclo(uint** conexiones, bool* nodos_ciclos, uint n);
void buscar_y_marcar_ciclo(uint** conexiones, bool* nodos_ciclos, lista &l, uint n);

void print_matriz(uint** M, uint m, uint n){
	for(uint i=0; i<m; i++){
		for(uint j=0; j<n; j++) cout << M[i][j];
		cout << endl;
	}
}

void limpiar_array(bool* array, uint n){
	for(int i=0;i<n;i++)
		array[i] = false;
}

template<class T> 
void print_vector(T* V, uint n){
	cout << "[ ";
	for(uint i=0; i<n; i++) cout << "(" << i+1 << "," << V[i] << ")";
	cout << "]";
	cout << endl;
}

template<class T>
void print_list(list<T> &l){
	typename list<T>::iterator it;
	cout << "		{ ";
	for(it=l.begin();it!=l.end();it++){
		cout << *it << " ";
	}
	cout << "}" << endl;
}

