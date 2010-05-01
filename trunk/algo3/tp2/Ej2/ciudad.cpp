#include<cmath>
#include<iostream>
#include<stack>
#include "../medicion.h"
#define pila stack<uint>

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

bool ciudad(uint** conexiones, uint n) {

	/** INSERT ALGORITHM HERE **/

	return true;
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
 