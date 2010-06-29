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
	for(int i=0; i<n; i++){
		if(V[i]) cout << "(" << i+1 <<"," << V[i] << ")";
	}
	cout << endl;
}

ostream& operator<<(ostream& out, const pair<int,int>& p )
{
	return out << "(" << p.first+1 << "," << p.second << ")";
}

int comparar( const void* _a, const void* _b )
{
	/*función de comparación para QUICKSORT*/
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
	/*Verifica si la variable nodo es adyacente a todos los vértices de la solución pertenece,
	*es decir, intenta agrandar la clique con 'nodo'	
	*/
	bool forma_completo = true;
	for( int j = 0 ; j < n ; j++ ){
		if(pertenece[j] && j!= nodo) {
			forma_completo &= adyacencia[nodo][j];
			O(4);
		}
		O(6);
	}
	if(forma_completo){
		pertenece[nodo] = true;
		tamanyo++;
		O(3);
	}
	O(3);
}

int constructivo(bool* pertenece, const pair<int,int>* d, int** adyacencia, const int n){
	if(n==0) return 0;
	/*construye una clique a partir del de mayor grado*/
	int tamanyo = 1;
	int actual = d[0].first;
	pertenece[actual] = true;
	O(7);
	for(int i=0;i<n;i++){
		int nodo = d[i].first;
		if(!pertenece[nodo]) formar_completo(pertenece,adyacencia,nodo,tamanyo,n);
		O(8);
	}
	O(1);
	return tamanyo;
}

//saca menor grado y pone mayor grado

int max_clique(bool* pertenece, int** adyacencia, int n){
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

	//Solución inicial
	int tamanyo = constructivo(pertenece,d,adyacencia,n);
	if(tamanyo==n || tamanyo==1) return tamanyo;
	O(4);
	int tam_actual 	       = tamanyo;
	int tam_mejor_vecindad = tam_actual;
	bool actual[n];
	bool mejor_vecindad[n];
	O(5);
	for(int i=0;i<n;i++){
		actual[i]	  = pertenece[i];
		mejor_vecindad[i] = pertenece[i];
		O(8);
	}
	
	bool mejore=true;
	O(1);
	while(mejore){
		mejore=false;
		O(4);
		for(int i=n-1;i>=0;i--){					//para cada nodo de menor a mayor grado pruebo sacarlo
			int v1=d[i].first;
			O(5);
			if(actual[v1]){						//si esta en la clique
				actual[v1]=false;				//lo saco
				tam_actual--;
				int nodo=-1;					//variable para reconstruir la solucion si no es posible mejorarla
				O(5);
				for(int j=0;j<n;j++){				//para cada nodo de mayor a menor grado pruebo agregarlos
					int v2=d[j].first;
					O(5);
					if(!actual[v2] && v2!=v1){		//si no pertenece a la clique y no es el que saque
						formar_completo(actual,adyacencia,v2,tam_actual,n);
						if(actual[v2]) {
							nodo=v2;		//me quedo en nodo el ultimo que agrego
							O(1);
						}
						O(2);
					}
					O(5);
				}
				if(tam_actual>tam_mejor_vecindad){		//si mejore con respecto al mejor de la vecindad, actualizo
					tam_mejor_vecindad=tam_actual;
					for(int j=0;j<n;j++) mejor_vecindad[j]=actual[j];
					O(5*n+2);
				} else{						//restauro la solucion anterior
					if(nodo!=-1){
						actual[nodo]=false;
						O(2);
					}
					actual[v1]=true;
					O(3);
				}
				O(2);
			}
			O(2);
		}
		if(tam_mejor_vecindad>tamanyo){					//si mejore con respecto al optimo actual, actualizo
			mejore=true;
			tamanyo=tam_mejor_vecindad;
			for(int j=0;j<n;j++) pertenece[j]=mejor_vecindad[j];
			O(5*n+3);
		}
		O(2);
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
