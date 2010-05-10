#include "../medicion.h"
#include <stack>
#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long ullint;
typedef stack<uint> pila;

void print_matriz(uint** M, uint m, uint n){
	for(uint i=0; i<m; i++){
		for(uint j=0; j<n; j++) cout << M[i][j];
		cout << endl;
	}
}

bool dfs_primer_ciclo(uint** conexiones, bool* nodos_ciclo, uint n )
{

	pila p;
	bool encontre_ciclo = false;
	bool visitado[n];
	uint nodo_actual;
	uint orden[n];
	uint tam_ciclo = 0;

	for(int i=0;i<n;i++)
	{
		orden[i] = n+1;
		visitado[i] = false;
		O(7);
	}

	p.push(0);
	visitado[0] = true;
	O(3);
	while( !encontre_ciclo && !p.empty() )
	{
		tam_ciclo++;
		nodo_actual = p.top();
		orden[nodo_actual] = tam_ciclo;
		p.pop();
		
		for( int i = 0 ; i < n && !encontre_ciclo ; i++ )
		{
			if( conexiones[nodo_actual][i]  )
				/* si es adyacente.. */
				if( !visitado[i] )
				{
					/* .. y no fue visitado, lo numero */
					p.push( i );
					visitado[i] = true;
					O(6);
				}
				else if( orden[i] < orden[nodo_actual]-1 ) 
				{
					/* .. fue visitado y no vengo de ahí, 
					 * entonces encontré un ciclo! */
					encontre_ciclo = true;
					/* dejo al nodo que comienza el ciclo arriba */
					p.push( i );
					O(3+6);
				}
			O(8);
		}
		O(12);
	}

	if( encontre_ciclo )
	{
		/* Marco los nodos del ciclo */
		for( int i = 0 ; i < n ; i++ )
		{
			nodos_ciclo[i] |= (orden[i]>=orden[p.top()] && orden[i]<n+1);
			O(13);
		}
	}

	return encontre_ciclo;

}

bool dfs_ciclo(uint** conexiones, uint desde, bool* nodos_ciclo, uint n )
{

	pila p;
	bool encontre_ciclo = false;
	bool visitado[n];
	uint nodo_actual;
	uint orden[n];
	uint tam_ciclo = 0;

	for(int i=0;i<n;i++)
	{
		orden[i] = n+1;
		visitado[i] = false;
		O(7);
	}

	p.push(desde);
	visitado[desde] = true;
	O(3);
	while( !encontre_ciclo && !p.empty() )
	{
		tam_ciclo++;
		nodo_actual = p.top();
		orden[nodo_actual] = tam_ciclo;
		p.pop();
		
		for( int i = 0 ; i < n && !encontre_ciclo ; i++ )
		{
			if( conexiones[nodo_actual][i] )
				/* si es adyacente.. */
				if( !visitado[i] )
				{
					/* .. y no fue visitado .. */
					if( nodos_ciclo[i] )
					{
						/* y era del ciclo original */
						encontre_ciclo = true;
						O(1);
					}
					p.push( i );
					visitado[i] = true;
					O(5);
				}
			O(8);
		}
		O(12);
	}

	if( encontre_ciclo )
	{
		/* Marco los nodos del ciclo */
		for( int i = 0 ; i < n ; i++ )
		{
			nodos_ciclo[i] |= (orden[i]<orden[p.top()]);
			O(9);
		}
	}
	O(1);

	return encontre_ciclo;
}

void adyacente_externo( uint** conexiones, bool* ciclo, int& nodo_busqueda, int& nodo_salida, int n )
{
	nodo_busqueda = -1;
	for( int j = 0 ; j < n && nodo_busqueda == -1 ; j++ )
	{
		if( ciclo[j] )
		for( int i = 0 ; i < n && nodo_busqueda == -1 ; i++ )
		{
			if( !ciclo[i] && conexiones[j][i] )
			{
				nodo_busqueda = i;
				nodo_salida = j;
				O(2);
				break;
			}
			O(10);
		}
		O(7);
	}
}

bool ciudad( uint** conexiones, int n )
{
	reiniciar_contador();
	bool ciclo[n];
	bool encontre_ciclo, termine;
	int nodo_busqueda, nodo_salida;
	int cant_nodos_ciclo = 0;
	int i,j;

	for( j=0; j<n; j++ )
		ciclo[j] = false;
	O(5*n);

	encontre_ciclo = dfs_primer_ciclo( conexiones, ciclo, n );

	termine = true;
	for( i = 0 ; i < n ; i++ )
		termine &= ciclo[i];
	O(6*n);

	while( !termine && encontre_ciclo )
	{
		adyacente_externo( conexiones, ciclo, nodo_busqueda, nodo_salida, n );
		conexiones[nodo_busqueda][nodo_salida] = 0;
		encontre_ciclo = dfs_ciclo( conexiones, nodo_busqueda, ciclo, n );
		conexiones[nodo_busqueda][nodo_salida] = 1;
		termine = true;
		O(4+8);
		for( i = 0 ; i < n ; i++ )
			termine &= ciclo[i];
		O(6*n);
	}

	return termine;
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
		if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
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

