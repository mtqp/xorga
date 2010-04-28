#include<iostream>
#include<queue>
#include <list>
#include <stack>
#include "../medicion.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

#define cola queue<uint>
#define lista list<uint>
#define pila stack<uint>
#define hoja -1
#define visitado 1
#define no_visitado 0

template<class T> void print_matriz(T** M, uint filas, uint columnas){
	for(uint i=0; i<filas; i++){
		for(uint j=0; j<columnas; j++) cout << M[j][i] << " ";
		cout << endl;
	}
}

template<class T> void print_vector(T* V, uint n){
	for(uint i=0; i<n; i++) cout << V[i]+1;
	cout << endl;
}

bool ciudad(uint** conexiones, uint n){
	int marcado[n];		//llevo la cuenta de los nodos vistos
	int padre[n];		//para cada nodo guardo su predecesor, es decir, el nodo que lo metio a la cola
	uint** predecesores;	//para cada nodo guarda la lista de sus predecesores
	predecesores = new uint* [n];
	for(uint i=0; i<n; i++){
		predecesores[i] = new uint [n];
		for(uint j=0; j<n; j++) predecesores[i][j]=0;
	}
	//inicializo todo
	for(uint i=0;i<n;i++){
		marcado[i]=no_visitado;
		padre[i]=-1;
		for(uint j=0;j<n;j++) predecesores[i][j]=false;
	}
	cola q;
	q.push(0);
	marcado[0]=visitado;
	while(!q.empty()){
		uint actual=q.front();
		q.pop();
		bool tiene_hijos=false;
		for(uint i=0;i<n;i++){
			if(conexiones[actual][i] && marcado[i]==0){ //si es adyacente y todavia no lo visite
				q.push(i);
				marcado[i]=visitado;
				padre[i]=actual;
				tiene_hijos=true;
			}
		}
		if(!tiene_hijos){
			marcado[actual]=hoja;
			uint i=actual;
			while(padre[i]>0){ //para cada hoja completo sus predecesores
				predecesores[i][padre[i]]=true;
				i=padre[i];
			}
		}
	}
	for(uint i=0;i<n;i++) if(marcado[i]==0) return false; //si el grafo no es conexo devuelvo falso
	lista l;
	//armo una lista con todas las hojas
	for(uint i=0;i<n;i++){
		if(marcado[i]==hoja) l.push_back(i);
		marcado[i]=0;
	}
	pila p;
	bool res=true;
	for(uint i=0;i<n;i++) padre[i]=-1;
	while(!l.empty() && res){
		uint actual=l.front();
		l.pop_front();
		bool cierra_ciclo=false;
		for(uint i=0;i<n;i++){	//como actual es una hoja si no tiene mas conexion que con su padre entonces no esta en un ciclo
			 if(conexiones[actual][i]==1){
				cierra_ciclo=true;
			}
		}
		if(!cierra_ciclo) return false;
		//veo si existe un camino desde la hoja hacia la raiz que no sea el de sus predecesores
		p.push(actual);
		marcado[actual]=1;
		bool existe_camino=false;
		while(!p.empty()){
			uint siguiente=p.top();
			p.pop();
			for(uint i=0;i<n;i++){
				if(conexiones[siguiente][i] && !marcado[i] && !predecesores[actual][i]){
					p.push(i);
					marcado[i]=1;
					padre[i]=siguiente;
					if(i==0) existe_camino=true;
				}
			}
		}
		res&=existe_camino;
		for(uint i=0;i<n;i++) marcado[i]=0;
	}
	return res;
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
