#include<iostream>
#include<string.h>
#include "../medicion.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

void print(const string* a, uint n){
	for(uint i=0; i<n; i++) cout << a[i] << endl;
}

uint programadores_en_simultaneo(const string* ingresos,const string* egresos,uint n){
	reiniciar_contador();
	uint max=0;	//max guarda la maxima cantidad de programadores en simultaneo hasta el momento
	uint tmp=0;	//tmp guarda la cantidad de programadores en simultaneo en un momento dado
	uint j=0,k=0;	//j se mueve dentro del array ingresos y k se mueve dentro del array de egresos
	O(4);
	while(j<n){	//cuando veo todos los ingresos ya me alcanza para decidir
		if(ingresos[j]<=egresos[k]){	//si ingresa alguien
			tmp++;			//sumo un programador a los que tengo hasta el momento
			j++;			//y paso al siguiente ingreso
			O(6);
		}
		else{				//si egresa alguien
			if(tmp>max) max=tmp;	//miro si la cantidad de programadores en simultaneo antes de que egrese ese programador es mayor a la maxima cantidad de programadores en simultaneo calculada anteriormente, si es asi actualizo max
			tmp--;			//resto un programador a los que tengo hasta el momento
			k++;			//y paso al siguiente egreso
			O(7);
		}
	}
	if(tmp>max) max=tmp;			//si es necesario actualizo el max
	O(2);
	return max;
}

int main (int argc, char** argv){
	ullint n;
	double ts=0;
	while(cin >> n && n!=(ullint)-1){
		string ingresos[n];
		string egresos[n];
		string delim;
		for(uint i=0; i<n; i++){ 
			cin >> ingresos[i];
			cin >> delim;
		}
		for(uint i=0; i<n; i++){
			 cin >> egresos[i];
		  	 cin >> delim;
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			medir_tiempo( ts, programadores_en_simultaneo(ingresos,egresos,n), 1, 0.5f );
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			programadores_en_simultaneo(ingresos,egresos,n);
			cout << n << "\t" << contador << endl; //imprimo la cuenta
		}
		else cout << programadores_en_simultaneo(ingresos,egresos,n) << endl;
	}
	return 0;
}