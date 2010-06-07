#include "Grafo.h"
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	double ts;
	int n;
	int cant_ady;
	int ady;
	while(cin >> n && n!=(int)-1){
		bool solucion[n];
		int** adyacencia;
		adyacencia = new int* [n];
		for(int i=0; i<n; i++){
			adyacencia[i] = new int [n];
			for(int j=0; j<n; j++){
				adyacencia[i][j]=0;
				solucion[j]=false;
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
			//medir_tiempo( ts, max_clique(solucion,adyacencia,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			//max_clique(solucion,adyacencia,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			Grafo g( n, adyacencia );
			cout << g.max_clique(solucion,adyacencia,n) << endl;
			cout << "N";
			print_res(solucion,n);
		}
		for(int i=0; i<n; i++){	//libero la memoria
			delete [] adyacencia[i];
		}
		delete [] adyacencia;
		}
	return 0;
}
