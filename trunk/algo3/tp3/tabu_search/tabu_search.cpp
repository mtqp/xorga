#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>
#include "../medicion.h"

using namespace std;

#define pair pair<int,int>
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
	for(int i=0; i<n; i++) cout << V[i] << ", ";
	cout << endl;
}

void print_lista(list l){
	list::iterator it;
	for(it=l.begin(); it!=l.end(); it++) cout << (*it)+1 << " ";
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

void rotar(list& l, int c){
	O(1);
	for(int q=0; q<c; q++){
		int primero = l.back();
		l.pop_back();
		l.push_front(primero);
		O(6);
	}
}

void formar_completo_lista(bool* pertenece,list& l,pair* d,int* tabu,int** adyacencia,int& tamanyo,const int n){
	list tmp;
	O(2);
	for(int i=0;i<n;i++){				//intento agregar de mayor a menor grado si todavía no pertenece a la clique y no esta tabú
		int nodo=d[i].first;
		O(5);
		if(!pertenece[nodo] && tabu[nodo]==0){
			bool forma_completo = true;

			/* Veo que el nodo que pretendo agregar sea adyacente a todos los que están en la clique
			** No uso la lista para hacer esto porque deberia ir agregando a la lista lo que pasa a pertenecer a la clique y eso no me permite ordenarlos de
			** menor a mayor entre ellos (los que logro agregar) y a la vez que esten al principio de la clique anterior */
			for(int j=0;j<n;j++){
				if(pertenece[j] && j!=nodo){
					forma_completo &= adyacencia[nodo][j];
					O(4);
				}
				O(6);
			}
			if(forma_completo){
				tmp.push_back(nodo);	//tmp contiene los nodos que logre agregar a la clique de mayor a menor grado
				pertenece[nodo]=true;
				tamanyo++;
				O(4);
			}
			O(3);
		}
		O(6);
	}
	//concateno las listas
	list::iterator it;
	it=l.begin();
	l.splice(it,tmp);
	O(3);
}

int constructivo(bool* pertenece, const pair* d, int** adyacencia, const int n){
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

int busqueda_local(bool* pertenece, pair* d, int** adyacencia, int n){
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
		actual[i] = pertenece[i];
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

/* CANTIDAD_ITERACIONES es la cantidad maxima de veces que dejo que mejore
** ITER_TABU es la cantidad de iteraciones que prohibo el atributo
** CANT_ITER es la cantidad maxima de iteraciones sin mejorar */

int max_clique_actual(bool* pertenece, int** adyacencia, int n, int MAX_ITERACIONES, int MAX_TABU_PORCENTAJE){
	// tupla (nodo, grado)	
	pair d[n];
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
	qsort( d, n, sizeof(pair), &comparar );
	O(n*n);

	//Solución inicial
	int tamanyo = busqueda_local(pertenece,d,adyacencia,n);
	if(tamanyo==1 || tamanyo==n) return tamanyo;
	int tam_actual;
	bool actual[n];
	int tabu[n];
	list clique_actual;
	
	bool mejore=true;
	O(5);
	while(mejore){
		mejore=false;
		int cant_iter=(tamanyo*MAX_TABU_PORCENTAJE/100);
		//cout << "cant_iter " << cant_iter << endl;
		/* Empiezo tamanyo veces desde la clique original a no ser que en alguna de las iteraciones logre mejorarla
		** Cada una de estas veces roto la lista para sacar los nodos en otro orden y asi explorar otros vecinos*/
		O(5);
		for(int c=0;c<tamanyo;c++){		
			int iteracion=0;					//iteración controla la máxima cantidad de veces que va a iterar el 'while' sin lograr mejorar
			//Inicializo todo
			tam_actual=tamanyo;
			for(int i=0;i<n;i++){
				actual[i]=false;
				tabu[i]=0;
				O(4);
			}
			clique_actual.clear();
			O(6);
			for(int i=0;i<n;i++){
				int vertice=d[i].first;
				if(pertenece[vertice]){
					clique_actual.push_back(vertice);	//quedan ordenados de mayor a menor grado
					actual[vertice]=true;
					O(3);
				}
				O(7);
			}
			//print_lista(clique_actual);
			//cout << "holaaaaa ------------------------------" << endl;
			rotar(clique_actual,c);
			
			//cout << "empiezo con Lista actual: ";
			//print_lista(clique_actual);
			//cout << "tam_actual: " << tam_actual << endl;

			while(tam_actual!=1 && !mejore && iteracion<MAX_ITERACIONES){
			
				/* Veifico si moviendome por la vecindad vuelvo a la clique inicial, si pasa eso salgo del 'while' para hacer la rotación y 
				   explorar soluciones distintas */
				O(12);
				bool igual_inicial=true;
				for(int i=0;i<n;i++){
					igual_inicial&=(pertenece[i]==actual[i]);
					O(7);
				}
				if(igual_inicial && iteracion!=0) break;
				
				/* Saco el nodo de menor grado (no en todas las iteraciones es el de menor grado sino que es el de menor grado entre los de la misma
				   antiguedad en la clique) de la clique actual */
				
				int nodo=clique_actual.back();					
				clique_actual.pop_back();			//saco el de menor grado
				actual[nodo]=false;
				tam_actual--;
				tabu[nodo]=cant_iter;
				O(8);
				//cout << "saco nodo: " << nodo+1 << endl;
				//cout << "tam_actual: " << tam_actual << endl;
				//cout << "queda Actual";
				//print_lista(clique_actual);  
				
				formar_completo_lista(actual,clique_actual,d,tabu,adyacencia,tam_actual,n);

				//cout << "despues de agregar";
				//print_lista(clique_actual);  
				//cout << "Tam_actual: " << tam_actual << endl;
				
				//cout<< "Tabu: ";
				//print_vector(tabu,n);
				
				if(tam_actual>tamanyo){				//si mejore, actualizo
					mejore=true;
					/* Si mejore, reseteo el arreglo 'tabu' e intento agrandar la clique (le doy la oportunidad de estar en la clique a los nodos
					   que estaban tabú) ya que poder agregar algo sirve porque consigo una clique todavía mayor */
					for(int i=0;i<n;i++){
						tabu[i]=0;
						O(4);
					}
					formar_completo_lista(actual,clique_actual,d,tabu,adyacencia,tam_actual,n);
					tamanyo=tam_actual;
					c=tamanyo;				//fuerzo salir del 'for' para empezar por la primero rotación
					O(5);
					for(int k=0;k<n;k++){
						pertenece[k]=actual[k];
						O(5);
					}
				}
				else{
					/* Si no pude mejorar, resto la cantidad de iteraciones que le queda a un nodo para dejar de ser tabú */
					O(1);
					for(int j=0;j<n;j++){
						if(tabu[j]>0){
							tabu[j]--;
							O(1);
						}
						O(4);
					}
				}
				iteracion++;
				O(3);
			}
		}
	}
	return tamanyo;
}

void parsearArgumentos( int argc, char**argv, int& modo, int& pMaxIter, int& pMaxTabu )
{
	modo = 0;
	pMaxIter = 60;
	pMaxTabu = 40;
	for( int i = 1 ; i < argc ; i++ )
	{
		string arg = string(argv[i]);
		if( arg=="iteraciones" && argc > i+1 )
		{
			i++;
			pMaxIter = atoi(argv[i]);
		}
		else if( arg=="tabu" && argc > i+1 )
		{
			i++;
			pMaxTabu = atoi(argv[i]);
		}
		else if( arg=="tamaño" )
			modo = 3;
		else if( arg=="time" )
			modo = 1;
		else if( arg=="count" )
			modo = 2;
	}
}

int main(int argc, char** argv){
	double ts;
	int n;
	int cant_ady;
	int ady;
	
	int modo, pMaxIter, pMaxTabu;
	parsearArgumentos( argc, argv, modo, pMaxIter, pMaxTabu ); 
	
	while(cin >> n && n!=(int)-1){
		bool pertenece[n];
		int** adyacencia;
		int MAX_ITERACIONES = n*pMaxIter/100;
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
		if(modo==1){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, max_clique_actual(pertenece,adyacencia,n,MAX_ITERACIONES,pMaxTabu), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(modo==2){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique_actual(pertenece,adyacencia,n,MAX_ITERACIONES,pMaxTabu);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else if(modo==3){	//si el argumento es "tamaño" no imprime los nodos de la clique
			cout << max_clique_actual(pertenece,adyacencia,n,MAX_ITERACIONES,pMaxTabu) << endl;
		}
		else{	//si no tiene argumento entonces imprime la solución
			//for( int z=0; z<n; z++ )
				//cout << z << "\t" << max_clique_actual(pertenece,adyacencia,n,z,n,n) << endl;
			cout << max_clique_actual(pertenece,adyacencia,n,MAX_ITERACIONES,pMaxTabu) << endl;
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