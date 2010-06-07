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

void formar_completo(bool* pertenece, int** adyacencia, const int nodo, int& tamanyo, const int n){
	bool forma_completo = true;
	for( int j = 0 ; j < n ; j++ )
		if(pertenece[j] && j!= nodo) forma_completo &= adyacencia[nodo][j];
		if(forma_completo){
			pertenece[nodo] = true;
			tamanyo++;
		}
}

int constructivo(bool* pertenece, const pair<int,int>* d, int** adyacencia, const int n){
	int tamanyo = 1;
	int actual = d[0].first;
	pertenece[actual] = true;
	
	for(int i=0;i<n;i++){
		int nodo = d[i].first;
		if(!pertenece[nodo]) formar_completo(pertenece,adyacencia,nodo,tamanyo,n);
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
	int tabu[n];
	for(int i=0;i<n;i++){
		actual[i]=pertenece[i];
		tabu[i]=0;
	}
	
	bool mejore=true;
	for(int k=0; k<n && mejore; k++){
		mejore=false;
		for(int i=0;i<n;i++){
			if(actual[i]){		//saco un nodo perteneciente a la solución actual
				//cout << "saco: " << i+1 << endl;
				tabu[i]=tamanyo-2;
				actual[i]=false;
				tam_actual--;
				for(int j=0;j<n;j++){
					//cout << "miro: " << j+1 <<  " " << tabu[j] << endl;
					if(!actual[j] && tabu[j]==0) formar_completo(actual,adyacencia,j,tam_actual,n);
				}
				if(tam_actual>tamanyo){		//si mejore, actualizo
					mejore=true;
					tamanyo=tam_actual;
					for(int j=0;j<n;j++) pertenece[j]=actual[j];
				}
			}
			for(int j=0;j<n;j++) if(tabu[j]>0) tabu[j]--;
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
		else if(argc>1 && string(argv[1])=="tamaño"){
			cout << max_clique(pertenece,adyacencia,n) << endl;
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
