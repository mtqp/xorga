#include <iostream>
#include "../medicion.h"
#include <queue>

using namespace std;

typedef unsigned int uint;

#define cola queue<uint>

template<class T> void print_vector(const T* A, uint n){
	for(uint i=0;i<n;i++) cout << A[i];
	cout << endl;
}

void print_matriz(uint** M, uint n){
	for(uint i=0;i<n;i++){
		for(uint j=0;j<n;j++) cout << M[i][j];
		cout << endl;
	}
}

bool libre(uint** conexiones, bool* tengo_llave, uint* puertas, uint n){
	reiniciar_contador();
	cola q;
	bool llegue = false;
	uint actual;
	bool visitados[n];
	bool enEspera[n];
	for(uint i=0;i<n;i++){
		visitados[i]=false;
		enEspera[i]=false;
		O(7);
	}
	q.push(0);
	visitados[0]=true;
	O(3);
	while( !q.empty() && !llegue ){
		actual=q.front();
		q.pop();
		for( uint i=0 ; i < n && !llegue ; i++ ){
			if( conexiones[actual][i] && tengo_llave[i] && !visitados[i]){
				tengo_llave[puertas[i]] = true;
				q.push(i);
				visitados[i]=true;
				llegue = (i == n-1);
				if(enEspera[puertas[i]]){
					q.push(puertas[i]);
					visitados[puertas[i]]=true;
					llegue = (n-1==puertas[i]);
					O(9);
				}
				O(12);
			} else {
				if(conexiones[actual][i] && !visitados[i]){
					enEspera[i]=true;
					O(2);
				}
				O(6);
			}
			O(13);
		}
		O(7);
	}
	return llegue;
}

int main(int argc, char** argv){
	double ts;
	uint n;
	uint p;
	uint m;
	while(cin >> n && n!=(uint)-1){
		cin >> p;
		cin >> m;
		bool tengo_llave[n];
		uint puertas[n];
		bool array_sin_llave[n];
		for(uint i=0;i<n;i++){
			tengo_llave[i]=true;
			puertas[i]=0;
		}
		for(uint i=0;i<p;i++){
			uint esta;
			cin >> esta;
			uint abre;
			cin >> abre;
			puertas[esta-1]=abre-1;
			tengo_llave[abre-1]=false;
		}
		//MATRIZ DE ADYACENCIA
		uint** conexiones;
		conexiones = new uint* [n];
		for(uint i=0;i<n;i++){
			conexiones[i] = new uint [n];
			for(uint j=0;j<n;j++) conexiones[i][j]=0;
		}
		uint k;
		uint q;
		for(uint i=0;i<m;i++){
			cin >> k;
			cin >> q;
			conexiones[q-1][k-1]=1;	
			conexiones[k-1][q-1]=1;
		}
		if( argc > 1 && string(argv[1])=="count"){
			libre(conexiones,tengo_llave,puertas,n);
			cout << n << "\t" << contador << endl;
		}
		else if( argc > 1 && string(argv[1])=="count_llaves"){
			libre(conexiones,tengo_llave,puertas,n);
			cout << p << "\t" << contador << endl;
		}
		else{
			if(libre(conexiones,tengo_llave,puertas,n)) cout << "libre" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0;i<n;i++) delete [] conexiones[i];
		delete [] conexiones;
	}
	return 0;
}
