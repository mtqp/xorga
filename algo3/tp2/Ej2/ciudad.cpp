#include<cmath>
#include<iostream>
#include<stack>
#include<queue>
#include<list>

#include "../medicion.h"
#define pila stack<uint>
#define cola queue<uint>
#define lista list<uint>
#define no_fuertemente_conexo 	false
#define fuertemente_conexo		true 

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

void print_vector(uint* V, uint n);
void print_matriz(uint** M, uint m, uint n);

bool ciudad(uint** conexiones, uint n);

bool dfs_busco_primer_ciclo(uint** conexiones, bool* nodos_ciclo, bool* nodos_ciclo_posible, uint n);
void buscar_y_marcar_ciclo(uint** conexiones, bool* nodos_ciclos, lista &l, uint n);
bool dame_adyacentes_a(uint nodo_actual,uint** conexiones, lista &l_ady, uint n);
void reseteo_matriz_salvo_ciclo(uint** conexiones, bool* nodos_ciclos, uint n);
void limpiar_array(bool* array, uint n);


/*
bool dame_arista_libre_ciclo(&nodo_salida, conexiones, nodos_ciclos, n)

bool formar_ciclo_desde(nodo_salida, conexiones, nodos_ciclos, &cant_nodos_ciclo, n);
*/
int main (int argc, char** argv){
	ullint n;
	double ts;
	uint cantidad_de_calles;
	uint esquina;
	while(cin >> n && n!=(ullint)-1){
		uint** conexiones;
		conexiones = new uint* [n];
		for(uint i=0; i<n; i++){
			conexiones[i] = new uint [n];
			for(uint j=0; j<n; j++) conexiones[i][j]=0;
		}
		for(uint i=0; i<n; i++){
			cin >> cantidad_de_calles;
			for(uint j=0; j<cantidad_de_calles; j++){
				cin >> esquina;
				conexiones[i][esquina-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, ciudad(conexiones,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			ciudad(conexiones,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			if(ciudad(conexiones,n)) cout << "fuertemente conexo" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0; i<n; i++){	//libero la memoria
			delete [] conexiones[i];
		}
		delete [] conexiones;
	}
	return 0;
}


bool ciudad(uint** conexiones, uint n) {
	
	//--chequear casos bases--//
	
	//--algun nodo tiene solo una arista--//
	
	//--VARIABLES--//
	/*crea array de nodos pertenecientes al ciclo*/
	bool nodos_ciclo [n];
	limpiar_array(nodos_ciclo, n);

	bool nodos_ciclo_posible [n];
	limpiar_array(nodos_ciclo_posible,n);

	/*se la cantidad de nodos q pertenecen al ciclo en cada momento*/
	uint cant_nodos_ciclo = 0;

	uint nodo_salida;
	bool  encontre_ciclo;
	//--CODIGO--//
	encontre_ciclo = dfs_busco_primer_ciclo(conexiones, nodos_ciclo, nodos_ciclo_posible, n);
	
	/*
	while((cant_nodos_ciclo < n) && encontre_ciclo){		//n-1???	
		if(dame_arista_libre_ciclo(&nodo_salida, conexiones, nodos_ciclos, n))
			encontre_ciclo = formar_ciclo_desde(nodo_salida, conexiones, nodos_ciclos, &cant_nodos_ciclo, n);
		else
			encontre_ciclo = false;			//fijarse si no puede pasar q cant_nodos sea N y esto da false
	}
	*/
	return encontre_ciclo;
}

bool dfs_busco_primer_ciclo(uint** conexiones, bool* nodos_ciclo, bool* nodos_ciclo_posible, uint n){
	pila  p;
	lista l;
	lista l_ady;
	
	bool encontre_ciclo = false;
	bool tiene_algo_para_agregar		= true;
	uint nodo_actual;
	
	p.push(0);									//push 1er nodo (EMPIRICAMENTE, el 0)			
	nodos_ciclo_posible[0] = true;	
	
	while((!encontre_ciclo) && tiene_algo_para_agregar /*&& (!c.empty())*/){
		nodo_actual = p.top();
		l.push_back(nodo_actual);
		p.pop();
		tiene_algo_para_agregar = dame_adyacentes_a(nodo_actual,conexiones, l_ady, n);
		
		while((!l_ady.empty()) && tiene_algo_para_agregar && (!encontre_ciclo)){
			if(!nodos_ciclo_posible[l_ady.front()]){
				p.push(l_ady.front());
				l_ady.pop_front();
				nodos_ciclo_posible[p.top()] = true;
			}else{
				l.push_back(l_ady.front());
				encontre_ciclo = true;
			}
		}
	}
	if(!tiene_algo_para_agregar) return no_fuertemente_conexo;
	if(encontre_ciclo){
		buscar_y_marcar_ciclo(conexiones, nodos_ciclo, /*nodos_ciclo_posibles, */l, n);
	}
	
	limpiar_array(nodos_ciclo_posible,n);

	return encontre_ciclo && tiene_algo_para_agregar;
}

bool dame_adyacentes_a(uint nodo_actual,uint** conexiones, lista &l_ady, uint n){
	for(int j=0; j<n; j++){
		if(conexiones[nodo_actual][j] == 1){
			conexiones[nodo_actual][j] = 2;
			conexiones[j][nodo_actual] = 2;
			l_ady.push_back(j);
		}
	}
	print_matriz(conexiones,n,n);
	cout << endl << endl;
	return !l_ady.empty();
}

void buscar_y_marcar_ciclo(uint** conexiones, bool* nodos_ciclos, /*bool* nodos_ciclo_posibles, */lista &l, uint n){
	bool encontre_ciclo = false;
	uint ultimo_int = l.back();
	uint primer_int;
	
	while(!encontre_ciclo){
		if(l.empty()) cout << "se comio la lista, agarrate q se viene el 0.25 seg_fault" << endl;
		primer_int = l.front();
		if(conexiones[ultimo_int][primer_int] == 2)
			encontre_ciclo = true;
		else
			l.pop_front();
	}
	cout << "esperemos q no seg faultee" << endl;
	while(!l.empty()){
		nodos_ciclos[l.front()] = true;
		cout << l.front()+1 << " ";
		l.pop_front();
	}
	cout << endl;
	
	reseteo_matriz_salvo_ciclo(conexiones, nodos_ciclos, n);
	
	cout << "resetee la matriz, qdo linda?" << endl;
	print_matriz(conexiones,n,n);

}

void reseteo_matriz_salvo_ciclo(uint** conexiones, bool *nodos_ciclos, uint n){
	for(int i=0; i<n; i++){
		if(nodos_ciclos[i]==0){
			for(int j=0;j<n;j++){
				if(conexiones[i][j] == 2){ 
					conexiones[i][j]=1;
					conexiones[j][i]=1;
				}
			}
		}
	}
}

void print_matriz(uint** M, uint m, uint n){
	for(uint i=0; i<m; i++){
		for(uint j=0; j<n; j++) cout << M[i][j];
		cout << endl;
	}
}

void print_vector(uint* V, uint n){
	for(uint i=0; i<n; i++) cout << V[i];
	cout << endl;
}

void limpiar_array(bool* array, uint n){
	/*costo = n*/
	for(int i=0;i<n;i++)
		array[i] = false;
}
