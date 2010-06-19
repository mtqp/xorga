#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>
#include "../medicion.h"

using namespace std;

#define lista list<int>

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

template<class T> void print_lista(T l){
	lista::iterator it;
	for(it=l.begin(); it!=l.end(); it++) cout << *it+1 << " ";
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

int busqueda_local(bool* pertenece, pair<int,int>* d, int** adyacencia, int n){
	//Solución inicial
	int tamanyo = constructivo(pertenece,d,adyacencia,n);
	int tam_actual = tamanyo;
	int tam_mejor_vecindad = tam_actual;
	bool actual[n];
	bool mejor_vecindad[n];
	for(int i=0;i<n;i++){
		actual[i]=pertenece[i];
		mejor_vecindad[i] = pertenece[i];
	}
	
	bool mejore=true;
	for(int k=0; k<n && mejore; k++){
		mejore=false;
		for(int i=0;i<n;i++){
			if(actual[i]){		//saco un nodo perteneciente a la solución actual
				//cout << "saco: " << i+1 << endl;
				actual[i]=false;
				tam_actual--;
				int nodo=-1;
				for(int j=0;j<n;j++){
					if(!actual[j] && j!=i){
						formar_completo(actual,adyacencia,j,tam_actual,n);
						if(actual[j]) nodo=j;
					}
				}
				//cout << "Actual: " << endl;
				//print_vector(actual,n);
				if(tam_actual>tam_mejor_vecindad){		//si mejore, actualizo
					tam_mejor_vecindad=tam_actual;
					for(int j=0;j<n;j++) mejor_vecindad[j]=actual[j];
				} else{
					if(nodo!=-1) actual[nodo]=false;
					actual[i]=true;
				}
			}
		}
		if(tam_mejor_vecindad>tamanyo){		//si mejore, actualizo
			mejore=true;
			tamanyo=tam_mejor_vecindad;
			for(int j=0;j<n;j++) pertenece[j]=mejor_vecindad[j];
		}
	}
	return tamanyo;
}
/*
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
	int tamanyo = busqueda_local(pertenece,d,adyacencia,n);
	int tam_actual = tamanyo;
	bool actual[n];
	int tabu[n];
	lista elem_tabu;
	lista::iterator it;
	for(int i=0;i<n;i++){
		actual[i]=pertenece[i];
		tabu[i]=0;
	}
	int cant_iter=max(tamanyo-2,3);
	bool mejore=true;
	for(int k=0; k<n && mejore; k++){
		mejore=false;
		for(int i=n-1;i>=0;i--){ //itero los nodos de menor grado a mayor grado
			if(actual[i]){		//saco un nodo perteneciente a la solución actual
				//cout << "saco: " << i+1 << "tabu: " << cant_iter << endl;
				actual[i]=false;
				tam_actual--;
				tabu[i]=cant_iter;
				//agrega cualquier nodo que no esté en la lista tabú 
				//y que no pertenezca a la solución original
				for(int j=0;j<n;j++){
					//cout << "miro: " << j+1 <<  " " << tabu[j] << endl;
					if(!actual[j] && tabu[j]==0) formar_completo(actual,adyacencia,j,tam_actual,n);
					//poner tabu al agregar??
				}
				
				//agrega los elementos de la lista tabú que 
				//se puedan agregar, y los saca de la lista
				it=elem_tabu.begin();
				while(it!=elem_tabu.end()){
					int nodo=*it;
					formar_completo(actual,adyacencia,nodo,tam_actual,n);
					it++;
					if(actual[nodo]){
						elem_tabu.remove(nodo);
						tabu[nodo]=0;
					}
				}
				//cout << "Actual: " << endl;
				//print_res(actual,n);
				// agrega el nodo actual a la lista tabú y actualiza
				//los valores de tabú de cada elemento
				elem_tabu.push_back(i);
				for(int j=0;j<n;j++)
					if(tabu[j]>0){
						tabu[j]--;
						if(tabu[j]==0) elem_tabu.remove(j);
					}
				//cout << "Lista tabu: " << endl;
				//print_lista(elem_tabu);
				// si encontró una solución mejor a la inicial,
				//la establece como solución actual
				if(tam_actual>tamanyo){		//si mejore, actualizo
					mejore=true;
					tamanyo=tam_actual;
					for(int j=0;j<n;j++) pertenece[j]=actual[j];
				}
			}
		}
	}
	return tamanyo;
}*/

int max_clique(bool* pertenece, int** adyacencia, int n, int CANTIDAD_ITERACIONES, int TABU_K ){
	// tupla (nodo, grado)
	pair<int,int> d[n];

	for(int i=0;i<n;i++){
		pertenece[i]=false;
		d[i].first  = i;
		d[i].second = 0;
		for( int j = 0 ; j < n ; j++ )
			if( adyacencia[i][j] )
				d[i].second++;
	}

	qsort( d, n, sizeof(pair<int,int>), &comparar );

	//Solución inicial
	int tamanyo = busqueda_local(pertenece,d,adyacencia,n);
	int tam_actual = tamanyo;
	bool actual[n];
	int tabu[n];
	lista elem_tabu;
	lista::iterator it;
	for(int i=0;i<n;i++){
		actual[i]=pertenece[i];
		tabu[i]=0;
	}
	bool mejore=true;
	for(int k=0; k<CANTIDAD_ITERACIONES && mejore; k++){
		mejore=false;
		for(int i=n-1;i>=0;i--){ //itero los nodos de menor grado a mayor grado
			int v1=d[i].first;	
			if(actual[v1]){		//saco un nodo perteneciente a la solución actual
				//cout << "saco: " << i+1 << "tabu: " << cant_iter << endl;
				actual[v1]=false;
				tam_actual--;
				tabu[v1]=max(tamanyo-TABU_K,3);
				/*while(n-1-elem_tabu.size()<=tamanyo && !elem_tabu.empty()){
					tabu[elem_tabu.front()]=0;
					elem_tabu.pop_front();
				}
				*/
				//agrega cualquier nodo que no esté en la lista tabú 
				// y que no pertenezca a la solución original
			
				for(int j=0;j<n;j++){
					int v2=d[j].first;
					//cout << "miro: " << j+1 <<  " " << tabu[j] << endl;
					if(!actual[v2] && tabu[v2]==0) formar_completo(actual,adyacencia,v2,tam_actual,n);
					//poner tabu al agregar??
				}
				
				//agrega los elementos de la lista tabú que 
				//se puedan agregar, y los saca de la lista
				  
				it=elem_tabu.begin();
				while(it!=elem_tabu.end()){
					int nodo=*it;
					formar_completo(actual,adyacencia,nodo,tam_actual,n);
					it++;
					if(actual[nodo]){
						elem_tabu.remove(nodo);
						tabu[nodo]=0;
					}
				}
				//cout << "Actual: " << endl;
				//print_res(actual,n);
				//agrega el nodo actual a la lista tabú y actualiza
				//los valores de tabú de cada elemento
				elem_tabu.push_back(v1);
				for(int j=0;j<n;j++)
					if(tabu[j]>0){
						tabu[j]--;
						if(tabu[j]==0) elem_tabu.remove(j);
					}
				//cout << "Lista tabu: " << endl;
				//print_lista(elem_tabu);
				//si encontró una solución mejor a la inicial,
				//la establece como solución actual  
				if(tam_actual>tamanyo){		//si mejore, actualizo
					mejore=true;
					tamanyo=tam_actual;
					for(int j=0;j<n;j++) pertenece[j]=actual[j];
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
			medir_tiempo( ts, max_clique(pertenece,adyacencia,n,n,2), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique(pertenece,adyacencia,n,n,2);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else if(argc>1 && string(argv[1])=="tamaño"){
			cout << max_clique(pertenece,adyacencia,n,n,2) << endl;
		}
		else{
			/*for( int z=0; z<n; z++ )
				cout << z << "\t" << max_clique(pertenece,adyacencia,n,z,2) << endl;*/
			cout << max_clique(pertenece,adyacencia,n,1,2) << endl;
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
