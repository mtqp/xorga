#include <iostream>
#include <cstdlib>
#include <utility>
#include "../medicion.h"

using namespace std;

template<class T> void print_res(const T V, int n){
	for(int i=0; i<n; i++)
		if(V[i]) cout << " " << i+1;
	cout << endl;
}

template<class T> void print_matriz(T M, int n){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++) cout << M[i][j];
		cout << endl;
	}
}

template<class T> void print_vector(T V, int n){
	for(int i=0; i<n; i++) cout << V[i];
	cout << endl;
}

ostream& operator<<(ostream& out, const pair<int,int>& p )
{
	return out << "(" << p.first+1 << "," << p.second << ")";
}

int comparar( const void* _a, const void* _b )
{
	const pair<int, int>* a = (const pair<int,int>*)_a;
	const pair<int, int>* b = (const pair<int,int>*)_b;
	if( a->second == b->second )
		return 0;
	else if( a->second < b->second )
		return 1;
	else
		return -1;
}

int max_clique(bool* pertenece, int** adyacencia, int n){
	if(n==0) return 0;
	int tamanyo = 1;
	O(1);
	// tupla (nodo, grado)
	pair<int,int> d[n];
	O(1);
	//inicializo el arreglo de grados
	O(1);
	for( int i = 0 ; i < n ; i++ )
	{
		d[i].first  = i;
		d[i].second = 0;
		O(9);
		for( int j = 0 ; j < n ; j++ ){
			if( adyacencia[i][j] ){
				d[i].second++;
				O(2);
			}
			O(4);
		}
	}

	//ordeno el arreglo de grados segun los mismo de mayor a menor
	qsort( d, n, sizeof(pair<int,int>), &comparar );
	O(n*n);
	//agrego a la clique el de mayor grado
	int actual = d[0].first;
	pertenece[actual] = true;
	O(6);
	for( int i = 0 ; i < n ; i++ )	//para cada nodo de mayor a menor grado
	{
		int nodo = d[i].first;
		if( !pertenece[nodo] )	//si no esta en la clique intento agregarlo
		{
			bool forma_completo = true;
			for( int j = 0 ; j < n ; j++ ){	//para cada nodo verifico adyacencia con 'nodo'
				if( pertenece[j] && j != nodo ){
					forma_completo &= adyacencia[nodo][j];
					O(4);
				}
				O(6);
			}
			if( forma_completo ){
				pertenece[nodo] = true;
				tamanyo++;
				O(3);
			}
			O(3);
		}
		O(7);
	}
	O(1);
	return tamanyo;

}

int main(int argc, char** argv){
	double ts;
	int n;
	int cant_ady;
	int ady;
	while(cin >> n && n!=(int)-1){
		bool pertenece[n];
		int** adyacencia;
		adyacencia = new int* [n];
		for(int i=0; i<n; i++){
			adyacencia[i] = new int [n];
			for(int j=0; j<n; j++){
				adyacencia[i][j]=0;
				pertenece[j]=false;
			}
		}
		for(int i=0; i<n; i++){
			cin >> cant_ady;
			for(int j=0; j<cant_ady; j++){
				cin >> ady;
				adyacencia[i][ady-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, max_clique(pertenece,adyacencia,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique(pertenece,adyacencia,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			cout << max_clique(pertenece,adyacencia,n) << endl;
			cout << "N";
			print_res(pertenece,n);
		}
		for(int i=0; i<n; i++){	//libero la memoria
			delete [] adyacencia[i];
		}
		delete [] adyacencia;
		}
return 0;
}
