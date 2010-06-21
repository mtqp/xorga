#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>
#include "../medicion.h"

using namespace std;

#define pair pair<int,int>
#define list_pair list< pair > 


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

void print_lista(list_pair l){
	list_pair::iterator it;
	for(it=l.begin(); it!=l.end(); it++) cout << "(" << (*it).first+1 << "," << (*it).second << ") ";
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

/*
bool comparar_lista(const pair a, const pair b){
 return (a.second<b.second);
}
*/

void formar_completo(bool* pertenece, int** adyacencia, const int nodo, int& tamanyo, const int n){
	bool forma_completo = true;
	for( int j = 0 ; j < n ; j++ )
		if(pertenece[j] && j!= nodo) forma_completo &= adyacencia[nodo][j];
		if(forma_completo){
			pertenece[nodo] = true;
			tamanyo++;
		}
}

void rotar(list_pair& l, int c){
	for(int q=0; q<c; q++){
		pair primero = l.back();
		l.pop_back();
		l.push_front(primero);
	}
}

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
/*
void agregar_clique(bool* pertenece,list_pair& l,list_pair tmp,int& tamanyo){
	cout << "voy a agregar: ";
	print_lista(tmp);
	list_pair::reverse_iterator it_tmp;
	list_pair::iterator it;
	it=l.begin();
	for(it_tmp=tmp.rbegin();it_tmp!=tmp.rend();it_tmp++){
		pair par=*it_tmp;
		//cout << "inserto: " << par << endl;
		while(it!=l.end() && par.second<(*it).second) it++;
		l.insert(it,par);
		int nodo=par.first;
		pertenece[nodo] = true;
		tamanyo++;
		//cout << "insertado" << endl;
		//print_lista(l);
	}
	cout << "agregados: ";
	print_lista(l);
}
*/
void formar_completo_lista(bool* pertenece,list_pair& l,pair* d,int* tabu,int** adyacencia,int& tam_actual,const int tamanyo,const int n){
	bool copia[n];
	for(int k=0;k<n;k++) copia[k]=pertenece[k];
	list_pair tmp;
	for(int i=0;i<n;i++){
		int nodo=d[i].first;
		if(!copia[nodo] && tabu[nodo]==0){
			bool forma_completo = true;
			for(int j=0;j<n;j++)
				if(copia[j] && j!=nodo) forma_completo &= adyacencia[nodo][j];
				if(forma_completo){
					tmp.push_front(d[i]);
					copia[nodo]=true;
				}
		}
	}
	concatenar(pertenece,l,tmp,tam_actual);
}

void agrandar_clique(bool* pertenece,list_pair& t, list_pair& l,int** adyacencia,int& tamanyo,const int n){
	list_pair::iterator it_tabu;
	it_tabu=t.begin();
	while(it_tabu!=t.end() && !t.empty()){
		pair par=*it_tabu;
		//cout << "evaluo par: " << par << endl;
		int nodo=par.first;
		bool forma_completo = true;
		for(int j=0;j<n;j++) if(pertenece[j] && j!=nodo) forma_completo &= adyacencia[nodo][j];
		if(!forma_completo){
			t.remove(par);
			it_tabu=t.begin();
			//cout << "no formo completo";
			//print_lista(t);
		}
		else{
			pertenece[nodo]=true;
			it_tabu++;
			//cout << "formo completo" << endl;
		}
	}
	concatenar(pertenece,l,t,tamanyo);
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
	for(int k=0; k<n && mejore; k++){
		mejore=false;
		for(int i=0;i<n;i++){
			if(actual[i]){
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

int max_clique_actual(bool* pertenece, int** adyacencia, int n, int CANTIDAD_ITERACIONES, int ITER_TABU ){
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
	list_pair elem_tabu;
	list_pair::iterator it_tabu;
	list_pair clique_actual;
	
	int cant_iter=max(tamanyo,3);
	bool mejore=true;
	for(int MAX_ITER=0; MAX_ITER<CANTIDAD_ITERACIONES && mejore; MAX_ITER++){
		mejore=false;
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
						clique_actual.push_back(d[i]);
						actual[vertice]=true;
					}
				}
				rotar(clique_actual,c);
				
				//cout << "tam_actual: " << tam_actual << endl;
				//cout << "Lista clique_actual";
				//print_lista(clique_actual);
				while(tam_actual!=1 && !mejore && iteracion<n){
					pair par=clique_actual.back();
					int nodo = par.first;
					
					clique_actual.pop_back();
					actual[nodo]=false;
					tam_actual--;
					tabu[nodo]=cant_iter;
					elem_tabu.push_back(par);
					
					//cout << "saco par: " << par << endl;
					//print_vector(tabu,n);
					
					formar_completo_lista(actual,clique_actual,d,tabu,adyacencia,tam_actual,tamanyo,n);
					  
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
							int v=d[j].first;
							if(tabu[v]){
								tabu[v]--;
								if(tabu[v]==0) elem_tabu.remove(d[j]);
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
			medir_tiempo( ts, max_clique_actual(pertenece,adyacencia,n,n,2), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique_actual(pertenece,adyacencia,n,n,2);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else if(argc>1 && string(argv[1])=="tamaño"){
			cout << max_clique_actual(pertenece,adyacencia,n,n,2) << endl;
		}
		else{
			/*for( int z=0; z<n; z++ )
				cout << z << "\t" << max_clique_actual(pertenece,adyacencia,n,z,2) << endl;*/
			cout << max_clique_actual(pertenece,adyacencia,n,n,1) << endl;
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
