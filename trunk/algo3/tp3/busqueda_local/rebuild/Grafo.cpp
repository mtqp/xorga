#include "Grafo.h"

Grafo::Grafo( int n, int** adyacencia )
{
	this->n = n;

	// copia la matriz de adyacencia
	this->adyacencia = new int*[n];
	for( int i=0 ; i < n ; i++ )
	{
		this->adyacencia[i] = new int[n];
		for( int j=0 ; j < n ; j++ )
			this->adyacencia[i][j] = adyacencia[i][j];
	}

	// inicializa el arreglo de grados
	for( int i=0 ; i < n ; i++ )
	{
		grados[i].first  = i;
		grados[i].second = 0;
		for( int j = 0 ; j < n ; j++ )
			if( adyacencia[i][j] )
				grados[i].second++;
	}

	// ordena el arreglo por grado
	qsort( grados, n, sizeof(std::pair<int,int>), &_compararGrados );
}

Grafo::~Grafo()
{
	for( int i=0 ; i < n ; i++ )
	{
		delete[] adyacencia[i];
	}
	delete[] adyacencia;
}

int Grafo::_compararGrados( const void* _a, const void* _b )
{
	const std::pair<int,int>* a = (const std::pair<int,int>*)_a;
	const std::pair<int,int>* b = (const std::pair<int,int>*)_b;
	if( a->second == b->second )
		return 0;
	else if( a->second < b->second )
		return 1;
	else
		return -1;
}

Subgrafo Grafo::maxClique()
{
	Subgrafo subgrafo( *this, NULL );
	return subgrafo;
}

ostream& operator<<( ostream& out, const Subgrafo& s )
{
	out << "Not yet implemented!" << endl;
}
