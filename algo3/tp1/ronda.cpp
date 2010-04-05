#include<list>
#include<iostream>

using namespace std;

typedef list<int> ListaDeAmigas;

/*bool menorQDosAmigas (ListaDeAmigas *personas, int n){
	int i = 0;
	bool res = true;
	for (i; (i<n) && res;i++) {
		res = res && (persona[i].length() <= 1);
	}
	return res;
}

bool ronda( ListaDeAmigas* personas, int n )
{
	if (menorQDosAmigas(personas,n)){		//costo del if == O(n)
		return false;
	} else { 
		ListaDeAmigas::iterator inicio;
		ListaDeAmigas::iterator solucion[n];
		inicio = personas[0].begin();
			int i = 1;
		while (inicio != personas[0].end()){
			ListaDeAmigas::iterador it;
			it = personas[*inicio];				guarda al 1ro q apunta el prim elem
			solucion[i] = it;
			if (*solucion[0] == *solucion[ULTIMO]) {
				///pase x todos y volvi al principio, HICE UN CAMINO!
				return true;
			}
			if (algunaAmigaRepetida(solucion, ULTIMAAMIGA)) {
				no puedo seguir x este caso
				si tengo una nueva rama la cambio y modifico
				si NO tengo mas ramas, SUBO y hago lo mismo con la anterior
				
			} else {
				agrego la amiga al array SOLUCION y vuelvo a empezar
			}
			
		}
		return false;
	
	}
	
}*/

bool ronda( ListaDeAmigas* personas, bool* enRonda, int actual, int n )
{

	bool todasEnRonda=true;
	ListaDeAmigas::iterator it;
	
	for( int i = 0 ; i < n ; i++ )
		todasEnRonda&=enRonda[i];
		
	bool esAmigaDeRoot=false;
	for( it = personas[actual].begin() ; it != personas[actual].end() ; it++ )
		esAmigaDeRoot |= (*it == 0);
	
	if( esAmigaDeRoot && todasEnRonda )
	{
		return true;
	}

	for( it = personas[actual].begin() ; it != personas[actual].end() ; it++ )
	{
		if( !enRonda[*it] )
		{
			enRonda[*it]=true;
			if( ronda( personas, enRonda, *it, n ) )
			{
				cout << *it << " ";
				return true;
			}
			enRonda[*it]=false;
		}
	}
	
	return false;

}


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

