#include<cmath>
#include<iostream>
#include<queue>
#include "../medicion.h"
#define cola queue<uint>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

void print_matriz(uint** M, uint n){
	for(uint i=0; i<n; i++){
		for(uint j=0; j<n; j++) cout << M[i][j];
		cout << endl;
	}
}

void print_vector(uint* V, uint n){
	for(uint i=0; i<n; i++) cout << V[i];
	cout << endl;
}

//veo si hay mas de un grupo de amgigas, si es asi nunca se va a poder formar una ronda con las n chicas ya que las de un grupo no son amigas de la del otro
bool unico_grupo(uint** relaciones, uint n){
	bool marcadas[n];
	for(uint i=0; i<n; i++) marcadas[i]=false;
	O(n);
	cola q;
	q.push(0);
	marcadas[0]=true;
	O(3);
	while(!q.empty()){
		uint chica = q.front();
		q.pop();
		O(3);
		for(uint amiga=0; amiga<n; amiga++){
			if(relaciones[chica][amiga]==1 && !marcadas[amiga]){
				q.push(amiga);
				marcadas[amiga] = true;
				O(3);
			}
			O(5);
		}
	}
	bool res = true;
	for(uint i=0; i<n; i++) res&=marcadas[i];
	O(n);
	return res;
}

bool ronda_de_amigas(uint** relaciones, uint n){
	reiniciar_contador();
	//veo que cada chica tenga al menos 2 amigas
	bool todas_amigas_de_todas=true;
	for(uint i=0; i<n; i++){
		uint cantidad_de_amigas=0;
		for(uint j=0; j<n; j++){
			if(relaciones[i][j]==1) cantidad_de_amigas++;
			O(4);
		}
		if(cantidad_de_amigas<2) return false;
		todas_amigas_de_todas&=(cantidad_de_amigas==n-1);
		O(6);
	}
	if(todas_amigas_de_todas) return true;
	if(!unico_grupo(relaciones,n)) return false;

	uint solucion[n];
	solucion[0]=0;	//empiezo la ronda por la chica 0
	uint s=1;	//posicion del vector prometedor para la que estoy buscando la chica
	uint i=0;	//posicion en la matriz de la chica de la cual busco una amiga
	uint ant=0;	//para retroceder, la ultima amiga vista que no lleva a una solucion
	O(4);
	while(s!=0){	//si retrocede lo suficiente para intentar cambiar la primer chica es porque no hay forma de hacer la ronda, ya que es lo mismo empezar por cualquiera
		for(uint j=ant; j<n; j++){	//j recorre las amigas de i
			if(relaciones[i][j]==1){	//si j es amiga de i
				if(j==solucion[0] && s==n) return true;	//si ya estan todas las chicas en la ronda y las "puntas" son amigas entonces se puede formar la ronda
				bool esta=false;
				for(uint k=0; k<s && !esta; k++){	//k recorre el vector de la posible solucion viendo si la amiga que quiero agregar ya esta en la ronda
					if(solucion[k]==j){
						esta=true;
					}
					O(6);
				}
				if(!esta && s<n){	//si no esta y todavia no llego el momento de cerrar la ronda (no estan todas las chicas)
					solucion[s]=j;	//agrego la chica j a la ronda
					i=j;	//empiezo a buscar una amiga de j
					j=-1;	//miro desde la primer amiga de la que agregue
					s++;
					O(7);
				}
				O(8);
			}
			O(4);
		}
		s--;	//saco la ultima chica que meti a la ronda
		i=solucion[s-1];	//vuelvo a buscar una amiga de la anterior
		ant=solucion[s]+1;	//ant es la chica que saque, sigo buscando a partir de ahi
		O(6);
	}
	return false;
}

int main (int argc, char** argv){
	ullint n;
	double ts;
	uint cantidad_de_amigas;
	uint amiga;
	while(cin >> n && n!=(ullint)-1){
		uint** relaciones;
		relaciones = new uint* [n];
		for(uint i=0; i<n; i++){
			relaciones[i] = new uint [n];
			for(uint j=0; j<n; j++) relaciones[i][j]=0;
		}
		for(uint i=0; i<n; i++){
			cin >> cantidad_de_amigas;
			for(uint j=0; j<cantidad_de_amigas; j++){
				cin >> amiga;
				relaciones[i][amiga-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, ronda_de_amigas(relaciones,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			ronda_de_amigas(relaciones,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			if(ronda_de_amigas(relaciones,n)) cout << "ronda" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0; i<n; i++){	//libero la memoria
			delete [] relaciones[i];
		}
		delete [] relaciones;
		}
	return 0;
}
