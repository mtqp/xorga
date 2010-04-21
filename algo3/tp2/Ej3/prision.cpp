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


bool libre(uint** conexiones, bool* puedo_entrar, uint* puertas, uint n){
	cola q;
	bool a[n];
	uint accesos[n];
	bool marcados[n];
	uint adyacentes;	
	bool llegue = false;
	uint actual;

	for(uint i=0;i<n;i++)
	{
		marcados[i]=false;
		adyacentes=0;
		for( uint j=0; j<n; j++ )
			if( conexiones[i][j] )
				adyacentes++;
		accesos[i]=adyacentes;	
	}

	q.push(0);
	accesos[0]--;
	marcados[0]=true;

	while( !q.empty() && !llegue )
	{
		actual=q.front();
		q.pop();
		for( uint i=0 ; i < n && !llegue ; i++ )
		{
			a[i]=false;
			if( conexiones[actual][i] && puedo_entrar[i] && accesos[i]>0 )
			{
				//tal vez necesitamos preguntar cuál fue el último que fuimos
				a[i]=true;
				puedo_entrar[puertas[i]] = true;
				accesos[i]--;
				marcados[i]=true;
				llegue = (i == n-1);
				
			}
		}
		for( uint i=0 ; i < n ; i++ )
			if( a[i] && !marcados[i] )
				q.push(i);

		for( uint i=0 ; i < n ; i++ )
			if( a[i] && marcados[i] )
				q.push(i);
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
		bool puedo_entrar[n];
		uint puertas[n];
		for(uint i=0;i<n;i++){
			puedo_entrar[i]=false;
			puertas[i]=0;
		}
		puedo_entrar[0]=true;
		puedo_entrar[n-1]=true;
		for(uint i=0;i<p;i++){
			uint esta;
			cin >> esta;
			puedo_entrar[esta-1]=true;
			uint abre;
			cin >> abre;
			puertas[esta-1]=abre-1;
		}
		//cout << "Puertas: " << endl;
		//print_vector(puertas,n);
		//cout << "puedo_entrar: " << endl;
		//print_vector(puedo_entrar,n);
		/*MATRIZ DE INCIDENCIA
		uint** conexiones;
		conexiones = new uint* [n];
		for(uint i=0;i<n;i++){
			conexiones[i] = new uint [m];
			for(uint j=0;j<m;j++) conexiones[i][j]=0;
		}
		uint k;
		for(uint i=0;i<m;i++){
			cin >> k;
			conexiones[k-1][i]=1;
			cin >> k;
			conexiones[k-1][i]=1;
		}*/
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
		//cout << "Conexiones: " << endl;
		//print_matriz(conexiones,n);
		if(argc>1 && string(argv[1])=="time"){
			medir_tiempo( ts, libre(conexiones,puedo_entrar,puertas,n), 1, 0.5f );
			cout << n << "\t" << ts << endl;
		}
		else if( argc > 1 && string(argv[1])=="count"){
			libre(conexiones,puedo_entrar,puertas,n);
			cout << n << "\t" << contador << endl;
		}
		else{
			if(libre(conexiones,puedo_entrar,puertas,n)) cout << "libre" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0;i<n;i++) delete [] conexiones[i];
		delete [] conexiones;
	}
return 0;
}
