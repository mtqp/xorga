#include<cmath>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>

#define empezar_medicion(var) __asm__ __volatile__ ("rdtsc;mov %%eax, %0" : : "g" (var) );
#define terminar_medicion(var) __asm__ __volatile__ ("rdtsc;sub %0,%%eax;mov %%eax,%0" : : "g" (var) );
#define vvint vector<vector<int> >
#define vint vector<int>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

bool ronda_de_amigas(vvint relaciones, uint n){
	//veo que cada chica tenga al menos 2 amigas, se podria hacer al leer la entrada pero se mezcla para la complejidad
	for(uint i=0; i<n; i++){
		uint cantidad_de_amigas=0;
		for(uint j=0; j<n; j++){
			if(relaciones[i][j]==1) cantidad_de_amigas++;
		}
		if(cantidad_de_amigas<2) return false;
	}
	vint solucion (n,0); //al inicializarlo en 0 estoy empezando la ronda por la chica 0
	uint s=1; //posicion del vector prometedor para la que estoy buscando la chica
	uint i=0; //posicion en la matriz de la chica de la cual busco una amiga
	uint ant=0; //para retroceder, la ultima amiga vista que no lleva a una solucion
	while(s!=0){//si retrocede lo suficiente para intentar cambiar la primer chica es porque no hay forma de hacer la ronda, ya que es lo mismo empezar por cualquiera
		for(uint j=ant; j<n; j++){ //j recorre las amigas de i
			if(j==solucion[0] && s==n) return true; //si ya estan todas las chicas en la ronda y las "puntas" son amigas entonces se puede formar la ronda
			if(relaciones[i][j]==1){//si j es amiga de i
				bool esta=false;
				for(uint k=0; k<s && !esta; k++){//k recorre el vector de la posible solucion viendo si la amiga que quiero agregar ya esta en la ronda
					if(solucion[k]==j){
						esta=true;
					}
				}
				if(!esta && s<=n){ //si no esta y todavia no llego el momento de cerrar la ronda (no estan todas las chicas)
					solucion[s]=j; //agrego la chica j a la ronda
					i=j; //empiezo a buscar una amiga de j
					j=-1; //miro desde la primer amiga de la que agregue
					s++;
				}
			}
		}
		s--; //saco la ultima chica que meti a la ronda
		i=solucion[s-1]; //vuelvo a buscar una amiga de la anterior
		ant=solucion[s]+1; //ant es la chica que saque, sigo buscando a partir de ahi
	}
	return false;
}

int main (int argc, char** argv){
	ullint n;
	ullint ts;
	uint cantidad_de_amigas;
	uint amiga;
	while(cin >> n && n!=-1){
		//cout << "n: " << n << endl;
		vvint relaciones (n,vector<int>(n,0));
		for(uint i=0; i<n; i++){
			//cout << "i: " << i << endl;
			cin >> cantidad_de_amigas;
			//cout << "cant amigas: " << cantidad_de_amigas << endl;
			for(uint j=0; j<cantidad_de_amigas; j++){
				//cout << "j: " << j << endl;
				cin >> amiga;
				//cout << "amiga: " << amiga << endl;
				relaciones[i][amiga-1]=1;
			}
		}
	if(argc>1 && string(argv[1])=="time"){
			empezar_medicion(ts);
			cout << ronda_de_amigas(relaciones,n);
			cout << "\t\t[" << ts << "]" << endl;
			terminar_medicion(ts);
		}
	else{
		if(ronda_de_amigas(relaciones,n)==1) cout << "ronda" << endl;
		else cout << "no" << endl;
	}
	}
	return 0;
}
