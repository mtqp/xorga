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

	/** CASOS EN LOS QUE NO ANDA **
	 * - Cuando el nodo que tiene grado máximo no está en la clique máxima
	 * - El de grado máximo conectado a todos de grado 1 y a una clique de tamaño > 2
	 **/

	int tamanyo = 1;

	// tupla (nodo, grado)
	pair<int,int> d[n];

	for( int i = 0 ; i < n ; i++ )
	{
		d[i].first  = i;
		d[i].second = 0;
		for( int j = 0 ; j < n ; j++ )
			if( adyacencia[i][j] )
				d[i].second++;
	}

	qsort( d, n, sizeof(pair<int,int>), &comparar );

	int actual = d[0].first;
	pertenece[actual] = true;

	//for( int k = 0 ; k < n ; k++ ) no tiene sentido...si en algun momento no puedo meter un nodo cuando es elegido por primera vez, nunca voy a poder meterlo ya que los que estan en el conj son definitivos
	for( int i = 0 ; i < n ; i++ )
	{
		int nodo = d[i].first;
		if( !pertenece[nodo] )
		{
			bool forma_completo = true;
			for( int j = 0 ; j < n ; j++ )
				if( pertenece[j] && j != nodo )
					forma_completo &= adyacencia[nodo][j];
			if( forma_completo ){
				pertenece[nodo] = true;
				tamanyo++;
			}
		}
	}
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
