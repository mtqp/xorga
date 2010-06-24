#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>
#include "../medicion.h"

using namespace std;

#define pair pair<int,int>
#define list_pair list< pair > 
#define list list<int>


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

void print_lista(list l){
	list::iterator it;
	for(it=l.begin(); it!=l.end(); it++) cout << *it;
	cout << endl;
}

ostream& operator<<(ostream& out, const pair& p )
{
	return out << "(" << p.first+1 << "," << p.second << ")";
}

int comparar( const void* _a, const void* _b ){
	const pair* a = (const pair*)_a;
	const pair* b = (const pair*)_b;
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

void rotar(list& l, int c){
	for(int q=0; q<c; q++){
		int primero = l.back();
		l.pop_back();
		l.push_front(primero);
	}
}
/*
void concatenar(bool* pertenece,list_pair& l,list_pair tmp,int& tamanyo){
	list_pair::iterator it;
	list_pair::iterator it_tmp;
	for(it_tmp=tmp.begin();it_tmp!=tmp.end();it_tmp++){
		pertenece[(*it_tmp).first]=true;
		tamanyo++;
	}
	it=l.begin();
	l.splice(it,tmp);
}
*/
void formar_completo_lista(bool* pertenece,list& l,pair* d,int* tabu,int** adyacencia,int& tamanyo,const int n){
	list tmp;
	for(int i=0;i<n;i++){
		int nodo=d[i].first;
		if(!pertenece[nodo] && tabu[nodo]==0){
			bool forma_completo = true;
			for(int j=0;j<n;j++)
				if(pertenece[j] && j!=nodo) forma_completo &= adyacencia[nodo][j];
				if(forma_completo){
					tmp.push_back(nodo);	//tmp de mayor a menor grado
					pertenece[nodo]=true;
					tamanyo++;
				}
		}
	}
	list::iterator it;
	it=l.begin();
	l.splice(it,tmp);
}

void agrandar_clique(bool* pertenece,list& t, list& l,int** adyacencia,int& tamanyo,const int n){
	list::iterator it_tabu;
	it_tabu=t.begin();
	while(it_tabu!=t.end() && !t.empty()){
		int nodo=*it_tabu;
		//cout << "evaluo par: " << par << endl;
		bool forma_completo = true;
		for(int j=0;j<n;j++) if(pertenece[j] && j!=nodo) forma_completo &= adyacencia[nodo][j];
		if(!forma_completo){
			t.remove(nodo);
			it_tabu=t.begin();
			//cout << "no formo completo";
			//print_lista(t);
		}
		else{
			pertenece[nodo]=true;
			tamanyo++;
			it_tabu++;
			//cout << "formo completo" << endl;
		}
	}
	//concatenar(pertenece,l,t,tamanyo);
	list::iterator it;
	it=l.begin();
	l.splice(it,t);
}

int constructivo(bool* pertenece, const pair* d, int** adyacencia, const int n){
	int tamanyo = 1;
	int actual = d[0].first;
	pertenece[actual] = true;
	for(int i=0;i<n;i++){
		int nodo = d[i].first;
		if(!pertenece[nodo]) formar_completo(pertenece,adyacencia,nodo,tamanyo,n);
	}
	return tamanyo;;
}

int busqueda_local(bool* pertenece, pair* d, int** adyacencia, int n){
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
	while(mejore){
		mejore=false;
		for(int i=n-1;i>=0;i--){
			int v1=d[i].first;
			if(actual[v1]){		//saco un nodo perteneciente a la solución actual
				actual[v1]=false;
				tam_actual--;
				int nodo=-1;
				for(int j=0;j<n;j++){
					int v2=d[j].first;
					if(!actual[v2] && v2!=v1){
						formar_completo(actual,adyacencia,v2,tam_actual,n);
						if(actual[v2]) nodo=v2;
					}
				}
				//cout << "Actual: " << endl;
				//print_vector(actual,n);
				if(tam_actual>tam_mejor_vecindad){		//si mejore, actualizo
					tam_mejor_vecindad=tam_actual;
					for(int j=0;j<n;j++) mejor_vecindad[j]=actual[j];
				} else{
					if(nodo!=-1) actual[nodo]=false;
					actual[v1]=true;
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
**CANTIDAD_ITERACIONES es la cantidad maxima de veces que dejo que mejore
**ITER_TABU es la cantidad de iteraciones que prohibo el atributo
**CANT_ITER es la cantidad maxima de iteraciones sin mejorar*/

int max_clique_actual(bool* pertenece, int** adyacencia, int n){
	// tupla (nodo, grado)
	pair d[n];

	for(int i=0;i<n;i++){
		pertenece[i]=false;
		d[i].first  = i;
		d[i].second = 0;
		for( int j = 0 ; j < n ; j++ )
			if( adyacencia[i][j] )
				d[i].second++;
	}

	qsort( d, n, sizeof(pair), &comparar );

	//Solución inicial
	int tamanyo = busqueda_local(pertenece,d,adyacencia,n);
	if(tamanyo==1 || tamanyo==n) return tamanyo;
	int tam_actual;
	bool actual[n];
	int tabu[n];
	list elem_tabu;
	list::iterator it_tabu;
	list clique_actual;
	
	bool mejore=true;
	while(mejore){
		mejore=false;
		int cant_iter=max(tamanyo,3);
			for(int c=0;c<tamanyo;c++){
				int iteracion=0;
				tam_actual=tamanyo;
				for(int i=0;i<n;i++){
					actual[i]=false;
					tabu[i]=0;
				}
				clique_actual.clear();
				elem_tabu.clear();
				for(int i=0;i<n;i++){
					int vertice=d[i].first;
					if(pertenece[vertice]){
						clique_actual.push_back(vertice); //quedan de mayor a menor grado
						actual[vertice]=true;
					}
				}
				rotar(clique_actual,c);

				//print_lista(clique_actual);
				
				//cout << "tam_actual: " << tam_actual << endl;
				//cout << "Lista clique_actual";
				//print_lista(clique_actual);
				while(tam_actual!=1 && !mejore && iteracion<n){
					int nodo=clique_actual.back();
					//cout << "nodo: " << nodo+1 << endl;
					
					clique_actual.pop_back();	//saco el de menor grado
					actual[nodo]=false;
					tam_actual--;
					tabu[nodo]=cant_iter;
					elem_tabu.push_front(nodo);	//tabu esta de mayor a menor grado
					
					//cout << "saco par: " << par << endl;
					//print_vector(tabu,n);
					
					formar_completo_lista(actual,clique_actual,d,tabu,adyacencia,tam_actual,n);
					//cout << "despues de agregar" << endl;
					//print_lista(clique_actual);  
					//cout << "Tamanyo: " << tam_actual << " Actual: ";
					//print_res(actual,n);
					//cout<< "Tabu: " << endl;
					//print_lista(elem_tabu);
					
					if(tam_actual>tamanyo){		//si mejore, actualizo
						agrandar_clique(actual,elem_tabu,clique_actual,adyacencia,tam_actual,n);
						mejore=true;
						tamanyo=tam_actual;
						c=tamanyo;
						for(int k=0;k<n;k++) pertenece[k]=actual[k];
					}
					else{
						for(int j=0;j<n;j++){
							if(tabu[j]){
								tabu[j]--;
								if(tabu[j]==0) elem_tabu.remove(j);
							}
						}
					}
					iteracion++;
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
			medir_tiempo( ts, max_clique_actual(pertenece,adyacencia,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique_actual(pertenece,adyacencia,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else if(argc>1 && string(argv[1])=="tamaño"){
			cout << max_clique_actual(pertenece,adyacencia,n) << endl;
		}
		else{
			//for( int z=0; z<n; z++ )
				//cout << z << "\t" << max_clique_actual(pertenece,adyacencia,n,z,n,n) << endl;
			cout << max_clique_actual(pertenece,adyacencia,n) << endl;
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
