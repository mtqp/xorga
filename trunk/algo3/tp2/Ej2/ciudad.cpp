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

void print_matriz(uint** M, uint m, uint dos){
	for(uint i=0; i<m; i++){
		for(uint j=0; j<dos; j++) cout << M[i][j];
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

bool dfs_busco_primer_ciclo(uint** conexiones, bool* nodos_ciclo, bool* nodos_ciclo_posible, uint n){
	pila q;
	lista l;
	
	lista l_ady;
	
	bool encontre_ciclo = false;
	bool conexo 		= true;
	uint nodo_actual;
	
	c.push(0);									//push 1er nodo (EMPIRICAMENTE, el 0)			
	l.push_back(0);
	//nodos_ciclo_posible[0] = true;	
	
	while((!encontre_ciclo) && conexo /*&& (!c.empty())*/){
		nodo_actual = c.front();
		c.pop();
		nodos_ciclo_posible[nodo_actual] = true;
		conexo = dame_adyacentes_a(nodo_actual,conexiones, l_ady);
		while((!l_ady.empty()) && conexo && (!encontre_ciclo)){
			if(!nodos_ciclo_posible[l_ady.front()]){
				q.push(l_ady.front());
				l_ady.pop_front();
			}else{
				l.push(l_ady.pop_front());
				encontre_ciclo = true;
			}
		}
	}
	
	if(!conexo) return no_fuertemente_conexo;
	
	if(encontre_ciclo){
		marco_en(nodos_ciclos)
	}

	reseteo(nodos_ciclo_posible)
	
	return encontre_ciclo && conexo;

}
/*
bool dame_arista_libre_ciclo(&nodo_salida, conexiones, nodos_ciclos, n)

bool formar_ciclo_desde(nodo_salida, conexiones, nodos_ciclos, &cant_nodos_ciclo, n);
*/

bool ciudad(uint** conexiones, uint n) {
	
	//--chequear casos bases--//
	
	//--algun nodo tiene solo una arista--//
	
	//--VARIABLES--//
	/*crea array de nodos pertenecientes al ciclo*/
	nodos_ciclo<bool>[n];
	limpiar_array(nodos_ciclo, n);

	nodos_ciclo_posible<bool>[n];
	limpiar_array(nodos_ciclo_posible,n)

	/*se la cantidad de nodos q pertenecen al ciclo en cada momento*/
	uint cant_nodos_ciclo = 0;

	uint nodo_salida;
	bool  encontre_ciclo;
	//--CODIGO--//
	encontre_ciclo = dfs_busco_primer_ciclo(conexiones, nodos_ciclo, nodos_ciclos_posible, n);
	
	while((cant_nodos_ciclo < n) && encontre_ciclo){		//n-1???	
		if(dame_arista_libre_ciclo(&nodo_salida, conexiones, nodos_ciclos, n))
			encontre_ciclo = formar_ciclo_desde(nodo_salida, conexiones, nodos_ciclos, &cant_nodos_ciclo, n);
		else
			encontre_ciclo = false;			//fijarse si no puede pasar q cant_nodos sea N y esto da false
	}
	return encontre_ciclo;
}

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
 
