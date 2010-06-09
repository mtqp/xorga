#include <iostream>
#include <utility>
#include <cstdlib>

using namespace std;

class Subgrafo;

class Grafo
{
	public:
		// genera un grafo, dados el tamaño y la matriz de adyacencia
		Grafo( int n, int** adyacencia );
		~Grafo();

		// devuelve la cantidad de nodos del grafo
		int cantidadNodos() const;

		// devuelve un subgrafo que son los nodos que pertenecen al clique máximo
		Subgrafo maxClique();

	private:
		
		// cantidad de nodos
		int n;

		// matriz de adyacencia
		int **adyacencia;

		// arreglo de tupla (nodo, grado) ordenado de mayor grado a menor grado
		pair<int,int>* grados;

		// función auxiliar usada para comparar las tuplas de grados
		static int _compararGrados( const void* _a, const void* _b );
		
		// esto va a ser reemplazado por un function pointer (algún día..)
		Subgrafo _max_clique_constructivo( );
		
};

class Subgrafo
{
	public:
		Subgrafo( const Grafo& grafo, bool* nodos );
		~Subgrafo();

	private:
		bool* nodos;
		const Grafo* grafo;
		int n;
};

ostream& operator<<( ostream& out, const Subgrafo& g );

