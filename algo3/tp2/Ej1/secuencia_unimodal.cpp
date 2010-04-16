#include <iostream>
#include "../medicion.h"

using namespace std;

typedef int uint;

void print(const int* secuencia, uint n){
	cout << "[ ";
	for(uint i=0; i<n; i++) cout << secuencia[i] << " ";
	cout << "]" << endl;
}

void long_max_creciente(int* secuencia, int* max_long_creciente, uint n){
	max_long_creciente[0]=1;
	O(2);
	for(uint i=1;i<n;i++){
		uint max_long=0;
		for(uint j=i-1;j>=0;j--){
			if(secuencia[j]<secuencia[i] && max_long_creciente[j]>max_long) max_long=max_long_creciente[j];
			O(10);
		}
		max_long_creciente[i]=max_long+1;
		O(6);
	}
}

uint secuencia_unimodal(int* secuencia, uint n){
		reiniciar_contador();
		int max_long_creciente[n];	//para cada indice 'i' en 'secuencia' el mismo indice en 'max_long_creciente' me dice la longitud máxima de una subsecuencia creciente que se puede armar a la izquierda del i-esimo elemento de 'secuencia' incluyendolo.
		long_max_creciente(secuencia,max_long_creciente,n);
		int max_long_decreciente[n];	//para cada indice 'i' en 'secuencia' el indice n-i en 'max_long_decreciente' me dice la longitud máxima de una subsecuencia decreciente que se puede armar a la derecha del i-esimo elemento de 'secuencia' incluyendolo.
		int secuencia_invertida[n];
		for(uint i=0;i<n;i++) secuencia_invertida[i]=secuencia[n-1-i];
		O(4*n);	
		long_max_creciente(secuencia_invertida,max_long_decreciente,n);
		uint max_long=0;
		for(uint i=0;i<n;i++){
			uint creciente_izq_pico=max_long_creciente[i];
			uint decreciente_der_pico=max_long_decreciente[n-1-i];
			uint long_secuencia_unimodal=creciente_izq_pico+decreciente_der_pico-1;
			if(long_secuencia_unimodal>max_long) max_long=long_secuencia_unimodal;
			O(12);
		}
		return n-max_long;
}

int main(int argc, char** argv){
	uint n;
	double ts;
	while(cin >> n && n!=(uint)-1){
		int secuencia[n];
		for(uint i=0; i<n; i++) cin >> secuencia[i];
		if(argc>1 && string(argv[1])=="time"){
			//empezar_medicion(ts);
			medir_tiempo( ts, secuencia_unimodal(secuencia,n), 1, 0.5f );
			//terminar_medicion(ts);
			cout << n << "\t" << ts << endl;
		}
		else if( argc > 1 && string(argv[1])=="count"){
			secuencia_unimodal(secuencia,n);
			cout << n << "\t" << contador << endl;
		}
		else cout << secuencia_unimodal(secuencia,n) << endl;
	}
return 0;
}
