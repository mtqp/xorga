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

	pila q;
	uint actual;
	bool recorrio_todos;
	bool visitados[n];
	uint i;

	/** checkeo los grados de los nodos **/
	uint grado=2;
	for( i=0; i < n && grado > 1; i++ )
	{
		grado=0;
		for( uint j=0; j < n ; j++ )
			if( conexiones[i][j] )
				grado++;
	}
	/** si algún nodo tenía grado menor que 2, no es FC **/
	if( grado<2 )
		return false;

	/** inicializo los nodos como no-visitados **/
	for( i=0; i < n; i++ )
		visitados[i] = false;

	/** meto el primer nodo para visitar **/
	q.push(0);
	visitados[0]=true;

	/** hago el DFS completo **/
	while( !q.empty() )
	{
		actual = q.top();
		q.pop();
		for( i=0; i < n; i++ )
			if( conexiones[actual][i] && !visitados[i] )
			{
				q.push(i);
				visitados[i]=true;
				/** oriento la arista **/
				conexiones[i][actual]=false;
			}
	}

	/** reinicializo visitados y checkeo si el DFS recorrió
	 *  todos los nodos.
	 */
	recorrio_todos=visitados[0];
	for( i=0; i < n ; i++ )
	{
		recorrio_todos &= visitados[i];
		visitados[i]=false;
	}

	/** si no recorrió todos, había más de una componente 
	 *  conexa, por lo tanto no es fuertemente conexo 
	 **/
	if( !recorrio_todos )
		return false;

	/** doy vuelta las orientaciones sólo si la arista estaba orientada **/
	for( i=0; i < n ; i++ )
		for( uint j=0; j < i; j++ )
			if( conexiones[i][j] xor conexiones[j][i] )
			{
				conexiones[i][j]=!conexiones[i][j];
				conexiones[j][i]=!conexiones[j][i];
			}

	/** empiezo el DFS por el último nodo del DFS anterior **/
	q.push(actual);
	visitados[actual]=true;
	while( !q.empty() )
	{
		actual = q.top();
		q.pop();
		for( i=0; i < n; i++ )
			if( conexiones[actual][i] && !visitados[i] )
			{
				q.push(i);
				visitados[i]=true;
			}
	}

	/** checkeo si recorrió todos los nodos **/
	recorrio_todos=visitados[0];
	for( i=0; i < n ; i++ )
		recorrio_todos &= visitados[i];

	/** si recorrió todos los nodos, es FC **/
	return recorrio_todos;
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
 
