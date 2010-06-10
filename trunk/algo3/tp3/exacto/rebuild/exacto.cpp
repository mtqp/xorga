#include <iostream>
#include "../../medicion.h"
#include "../../misc.h"

using namespace std;

void construir_solucion(bool* actual, int** adyacencia, const int n_maximo, int& n_actual, int& nodo, const int n){  //construyo una solución a partir de 'ult' considerendo desde 'sig'
	/**
	  * Construye una solución a partir del nodo actual
	  **/
	for(int j=nodo+1 ; j<n && n_maximo < n_actual+n-j ; j++){
		if(adyacencia[nodo][j]){	//si son adyacentes
			bool completo=true;
			for(int k=0;k<j && completo;k++) { //veo que este conectado a todos los que conforman la solución parcial
				if(actual[k] && !adyacencia[k][j]) 
					completo=false;
			}
			if(completo){
				actual[j]=true;	//lo agrego a la solución
				nodo=j;		//pasa a ser el último en la rama
				n_actual++;
			}
		}
	}
}

bool retroceder(bool* actual, int& cant, int& ult, int& sig){
	actual[ult]=false; 	//saco el último, 'retrocedo'
	cant--;
	sig=ult+1;		//sigo viendo a partir del que acabo de sacar
	bool encontre_ant=false;
	for(int j=ult-1;j>=0 && !encontre_ant;j--){	//busco el nuevo ultimo
		if(actual[j]){
			ult=j;
			encontre_ant=true;
		}
	}
	return encontre_ant;
}

int max_clique(bool* solucion, int** adyacencia, int n){
	int  n_maximo=0;	//indica la cantidad de nodos que tiene la clique máxima hasta el momento
	bool actual[n];		//contiene el subgrafo completo actual
	int  nodo;			//señala el nodo del cual busco un adyacente
	int  sig;			//indica a partir de cual lo busco

	/**
	  * Busca cliques a partir de cada nodo ya que si fijara un nodo
	  * inicial, la solución podría no incluir ese nodo
	  */
	for( int i=0 ; i < n ; i++ ){	//tengo que empezar una vez por cada uno ya que la solución final podría no incluir el nodo inicial
		for(int j=0 ; j<n ; j++) 
			actual[j]=false;		//reseteo la solución parcial
		
		int n_actual=1;				//cantidad de nodos de la solución parcial
		actual[i]=true;				//lo marco como parte de la solución
		nodo=i;						//último nodo en la rama que se esta explorando, en este momento el inicial
		sig=nodo+1;
		bool encontre_ant=true;
		while(encontre_ant){		//cuando retrocedo..veo si lo que intente sacar es el nodo inicial
			construir_solucion(actual, adyacencia, n_maximo, n_actual, nodo, n);
			if(n_actual > n_maximo){ 		//si la solucián parcial tiene más nodos que la mejor solución encontrada hasta el momento, actualizo 
				n_maximo=n_actual;
				for(int j=0;j<n;j++) 
					solucion[j]=actual[j]; //establece la solución encontrada como la mejor hasta el momento
			}
			encontre_ant=retroceder(actual,n_actual,nodo,sig);
		}
	}
	return n_maximo;
}

int main(int argc, char** argv){
	double ts;
	int n;
	int cant_ady;
	int ady;
	while(cin >> n && n!=(int)-1){
		bool pertenece[n];
		int** adyacencia;
		adyacencia = new int* [n];
		for(int i=0; i<n; i++){
			adyacencia[i] = new int [n];
			for(int j=0; j<n; j++){
				adyacencia[i][j]=0;
				pertenece[j]=false;
			}
		}
		for(int i=0; i<n; i++){
			cin >> cant_ady;
			for(int j=0; j<cant_ady; j++){
				cin >> ady;
				adyacencia[i][ady-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, max_clique(pertenece,adyacencia,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			max_clique(pertenece,adyacencia,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else if(argc>1 && string(argv[1])=="tamaño"){
			cout << max_clique(pertenece,adyacencia,n) << endl;
		}
		else{
			cout << max_clique(pertenece,adyacencia,n) << endl;
			cout << "N";
			print_res(pertenece,n);
		}
		for(int i=0; i<n; i++){	//libero la memoria
			delete [] adyacencia[i];
		}
		delete [] adyacencia;
		}
return 0;
}
