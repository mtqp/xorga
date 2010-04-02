#include<list>
#include<iostream>

using namespace std;

typedef list<int> ListaDeAmigas;

bool menorQDosAmigas (ListaDeAmigas *personas, int n){
	int i = 0;
	bool res = true;
	for (i; (i<n) && res;i++) {
		res = res && (persona[i].length() <= 1);
	}
	return res;
}

bool algunaAmigaRepetida(ListaDeAmigas::iterator *solucion, int longitud){
	int i = 0;
	bool repetida = false;
	for(i;i<longitud && (!res);i++){
		repetida = *(solucion[i]) == *(solucion[longitud]);
	}
	return repetida;
}

bool ronda( ListaDeAmigas* personas, int n )
{
	if (menorQDosAmigas(personas,n)){		//costo del if == O(n)
		return false;
	} else { 
		ListaDeAmigas::iterator inicio;
		ListaDeAmigas::iterator solucion[n+1];
		inicio = personas[0].begin();
		int i = 1;
		ListaDeAmigas::iterador it;
		it = personas[*inicio];				//guarda al 1ro q apunta el prim elem
		solucion[i] = it;
			
		while (inicio != personas[0].end()){
			
			agrego con iterador en la pos I
			if (1 == *solucion[ULTIMO]//valor en N+1) {
				///pase x todos y volvi al principio, HICE UN CAMINO!
				return true;
			}
			if (nivel I se me vacia) {
				borro niv i
				i--;
			}
			if (algunaAmigaRepetida(solucion, i)) { y tbm falta agregar si el nivel se me vacia
				borro niv i
				i--;
				if (*solucion[i].end()){
					borro niv i
					i--;
				} else {
				  	solucion[i] = solucion[i].SIGUIENTEITERADOR(); 	
				}
			} else {
				creo el siguiente iterador y vuelvo a empezar con ese
				i++
			}
		}
		return false;
	
	}
	
}
/*
bool ronda( ListaDeAmigas* personas, bool* pasadas, int actual, int n )
{
	//cout << "Actual: " << actual << endl;
	bool todasPasadas=true;
	ListaDeAmigas::iterator it;
	
	for( int i = 0 ; i < n ; i++ )
		todasPasadas&=pasadas[i];
		
	bool esAmigaDeRoot=false;
	for( it = personas[actual].begin() ; it != personas[actual].end() ; it++ )
		esAmigaDeRoot |= (*it == 0);
	
	if( esAmigaDeRoot && todasPasadas )
	{
		return true;
	}	
	for( it = personas[actual].begin() ; it != personas[actual].end() ; it++ )
	{
		if( !pasadas[*it] )
		{
			pasadas[*it]=true;
			if( ronda(personas, pasadas, *it, n ) )
			{
				return true;
			}
			pasadas[*it]=false;
		}
	}
	
	return false;

}
*/

int main( ) 
{
	int n;
	int a, cant_amigas;
	while(cin >> n && n!=-1){
		ListaDeAmigas personas[n];
		bool p[n];
		cout << "Son " << n << " amigas" << endl;
		for( int i = 0 ; i < n ; i++ )
		{
			cin >> cant_amigas;
			cout << "La persona " << i << " tiene " << cant_amigas <<": ";
			p[i]= false;
			for( int j = 0 ; j < cant_amigas ; j++ )
			{
				cin >> a;
				cout << a << " ";
				personas[i].push_back( a-1 );
			}
			cout << endl;
		}
		p[0]=true;
		cout << "Es ronda: " << ronda( personas, p, 0, n ) << endl;
	}
	
	return 0;
}

