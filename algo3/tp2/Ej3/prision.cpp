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
	bool puedo_entrar[n];
	uint accesos[n];
	bool visitados[n];
	uint adyacentes;
	bool llegue = false;
	uint actual;
	O(7);
	for(uint i=0;i<n;i++)
	{
		visitados[i]=false;
		adyacentes=0;
		for( uint j=0; j<n; j++ )
			if( conexiones[i][j] )
				adyacentes++;
		accesos[i]=adyacentes;
		O(12);
	}

	q.push(0);
	accesos[0]--;
	visitados[0]=true;
	O(5);
	while( !q.empty() && !llegue )
	{
		actual=q.front();
		q.pop();
		for( uint i=0 ; i < n && !llegue ; i++ )
		{
			puedo_entrar[i]=false;
			if( conexiones[actual][i] && tengo_llave[i] && accesos[i]>0 )
			{
				puedo_entrar[i]=true;
				tengo_llave[puertas[i]] = true;
				accesos[i]--;
				visitados[i]=true;
				llegue = (i == n-1);
				O(10);
			}
			O(18);
		}
		for( uint i=0 ; i < n ; i++ )
			if( puedo_entrar[i] && !visitados[i] ){
				q.push(i);
				O(6);
			}

		for( uint i=0 ; i < n ; i++ )
			if( puedo_entrar[i] && visitados[i] ){
				q.push(i);
				O(6);
			}
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
		if(argc>1 && string(argv[1])=="time"){
			medir_tiempo( ts, libre(conexiones,tengo_llave,puertas,n), 1, 0.5f );
			cout << n << "\t" << ts << endl;
		}
		else if( argc > 1 && string(argv[1])=="count"){
			libre(conexiones,tengo_llave,puertas,n);
			cout << n << "\t" << contador << endl;
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
