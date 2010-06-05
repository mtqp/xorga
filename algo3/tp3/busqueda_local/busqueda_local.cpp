#include <iostream>
#include <cstdlib>
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
	const pair<int,int>* a = (const pair<int,int>*)_a;
	const pair<int,int>* b = (const pair<int,int>*)_b;
	if( a->second == b->second )
		return 0;
	else if( a->second < b->second )
		return 1;
	else
		return -1;
}

bool forma_completo(bool* pertenece, const int nodo, int** adyacencia, const int n){
	bool forma_completo = true;
	for( int j = 0 ; j < n ; j++ )
		if(pertenece[j] && j!= nodo) forma_completo &= adyacencia[nodo][j];
	return forma_completo;
}

int constructivo(bool* pertenece, const pair<int,int>* d, int** adyacencia, const int n){
	int tamanyo = 1;
	int actual = d[0].first;
	pertenece[actual] = true;
	
	for(int i=0;i<n;i++){
		int nodo = d[i].first;
		if(!pertenece[nodo])
			if(forma_completo(pertenece,nodo,adyacencia,n)){
				pertenece[nodo] = true;
				tamanyo++;
			}
	}
	return tamanyo;
}

int max_clique(bool* pertenece, int** adyacencia, int n){
	// tupla (nodo, grado)
	pair<int,int> d[n];

	for(int i=0;i<n;i++){
		d[i].first  = i;
		d[i].second = 0;
		for( int j = 0 ; j < n ; j++ )
			if( adyacencia[i][j] )
				d[i].second++;
	}

	qsort( d, n, sizeof(pair<int,int>), &comparar );

	//Solución inicial
	int tamanyo = constructivo(pertenece,d,adyacencia,n);
	int tam_actual = tamanyo;
	bool actual[n];
	for(int i=0;i<n;i++) actual[i]=pertenece[i];
	
	bool mejore=true;
	int k=0;
	while(mejore && k<n){
		mejore=false;
		for(int i=0;i<n;i++){
			bool agregue=false;
			if(actual[i]){		//saco un nodo perteneciente a la solución actual
				actual[i]=false;
				tam_actual--;
				for(int j=0;j<n;j++){
					if(!actual[j] && j!=i)
						if(forma_completo(actual,j,adyacencia,n)){
							agregue=true;
							actual[j] = true;
							tam_actual++;
						}
				}
				if(tam_actual>tamanyo){		//si mejore, actualizo
					mejore=true;
					tamanyo=tam_actual;
					for(int j=0;j<n;j++) pertenece[j]=actual[j];
				}
				else if(!agregue){
					tam_actual=tamanyo;
					actual[i]=true;
				}
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
