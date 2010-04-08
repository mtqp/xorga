#include<list>
#include<iostream>

using namespace std;

typedef list<int> ListaDeAmigas;

////////////////////////////FUNCIONES AUXILIARES/////////////////////
void imprimir_resultado(ListaDeAmigas :: iterator *solucion, int longitud_solucion){
	int i = 0;
	for(i;i<longitud_solucion;i++){
		cout << *solucion[i] << ",";
	}
	cout << endl;
}

bool ronda_posible(ListaDeAmigas :: iterator *solucion, int longitud_solucion){
	return *solucion[longitud_solucion-1] == 0;
}
	
bool dame_ult_it_not_NULL(ListaDeAmigas :: iterator *solucion, ListaDeAmigas *personas, int & retroceder_hasta, ListaDeAmigas :: iterator & it_actual){
	bool encontre_camino = false;
	while (!encontre_camino){
		/*en solucion[i-1] tengo el iterador de la chica i-1 a sus amigas
		**tengo q ver si ese iterador no es nulo, si es sigo subiendo
		**si no, modifico solucion[i-1] con el iterador siguiente y devuelvo true*/
		retroceder_hasta--;
		
		if (retroceder_hasta == -1){		//me pase y se me acabo el arreglo
			cout << "PUM, ME ESCAPE" << endl;
			return true;
		}

		solucion[retroceder_hasta]++;
		if (retroceder_hasta == 0){
			if(solucion[retroceder_hasta] != personas[0].end()){
				it_actual = solucion[retroceder_hasta]++;
				return false;
			}
		}else {
			if(solucion[retroceder_hasta] != personas[*solucion[retroceder_hasta-1]].end()){
				it_actual = solucion[retroceder_hasta]++;
				return false;
			}
		}
		
	}
}

bool menorQDosAmigas (ListaDeAmigas *personas, int n){
	int i = 0;
	bool res = true;
	for (i; (i<n) && res;i++) {
		res = res && (personas[i].size() <= 1);
	}
	return res;
}

bool algunaAmigaRepetida(ListaDeAmigas::iterator *solucion, int longitud){
	/*se puede optimizar xq de precond son >= a tres 
	**la cant chicas y no son amigas de si mismas*/
	int i = 0;
	bool repetida = false;
	bool igual_cero = false;
	for(i;i<longitud && (!repetida) && (!igual_cero);i++){
		repetida = (*(solucion[i]) == *(solucion[longitud]));
		igual_cero =  (0 == *solucion[i]);
		///recordemos q la amiga cero solo puede aparecer en el indice i-1 y nada mas q ahi
	}
	cout << "IGUAL A CERO LOCOOOO == " << igual_cero << endl;
	return repetida || igual_cero;
}
//////////////////////////////////////////////////////////////////

bool ronda( ListaDeAmigas* personas, int n ){
	if (menorQDosAmigas(personas,n)){		//costo del if == O(n)
		return false;
	} else { 
		//Creo nuestro array solucion, donde se guardara el camino posible en cada momento//
		//el tam es n+1 ya que si [0] == [n+1] ==> cerre la vuelta / HAY RONDA //
		int longitud_solucion = n;
		ListaDeAmigas :: iterator solucion[longitud_solucion];

		//bools encargados de manejar ambos whiles//
		bool encontre_posible = false;
		bool it_apunta_NULL	  = false;
		bool probe_todas_posibilidades = false;
		
		//recorrera el arreglo 'solucion'//
		int i = 1;
		
		//iterador de solucion[i]//
		ListaDeAmigas :: iterator it_actual;
		it_actual = personas[0].begin();

		//guardo quien es la 1ra amiga la 1er chica//
		solucion[0] = it_actual;
		while (!probe_todas_posibilidades){
			
			//en solucion[i] guardo la primer amiga de la chica i-1;//
			solucion[i] = personas[*it_actual].begin();
			
			//it_actual se encarga de recorrer las posibles amigas de i-1//
			it_actual = solucion[i];	
			
			///WHILE 2///SE ENCARGA DE BUSCAR UNA POSIBLE O TERMINAR
			while(!encontre_posible && !it_apunta_NULL){
				//----IF 1---------//
				
				cout << "i == " << i << endl;
				imprimir_resultado(solucion,i);
				
				
				if(i == longitud_solucion) {						//mi arreglo solucion esta lleno!
					//---------------//
					if (ronda_posible (solucion,longitud_solucion)){	//hay ronda? //SIEMPRE ESTA SIN REP SALVO ULT==1ro
						return true;
					} else {
						it_actual++;								//existe otro camino para recorrer o tengo q subir?
						if (it_actual == personas[*solucion[i-1]].end()){
							it_apunta_NULL = true;
						} else {
							encontre_posible = true;
							solucion[i] = it_actual;
						}
					}
					//---------------//
				} else {
					//---------------//
					if (!algunaAmigaRepetida(solucion, i)){			//la nueva chica no estaba previamente en la ronda?
						encontre_posible = true;
					} else {
						it_actual++;								//existe otro camino para recorrer o tengo q subir?
						if (it_actual == personas[*solucion[i-1]].end()){
							it_apunta_NULL = true;
						
						} else {
							solucion[i] = it_actual;
						}
					}
					//---------------//
				}
				//----END IF 1-------//
			}
			//END WHILE 2//
			
			if (encontre_posible) {
				i++;												//probemos con el siguiente nivel!
			}
		
			if (it_apunta_NULL) {
				//vuelve hasta encontrar algun camino posible desde los padres
				//**y devuelve falso si llego hasta la raiz y no pudo encontrar
				//**otro camino
				probe_todas_posibilidades = !dame_ult_it_not_NULL(solucion,personas,i, it_actual);
			}
			
			encontre_posible= false;
			it_apunta_NULL 	= false;
		}
		return false;	
	}
	
}

int main( ) 
{
	int n;
	int a, cant_amigas;
	while(cin >> n && n!=-1){
		ListaDeAmigas personas[n];
		cout << "Son " << n << " amigas" << endl;
		for( int i = 0 ; i < n ; i++ )
		{
			cin >> cant_amigas;
			cout << "La persona " << i << " tiene " << cant_amigas <<": ";
			for( int j = 0 ; j < cant_amigas ; j++ )
			{
				cin >> a;
				cout << a-1 << " ";
				personas[i].push_back( a-1 );
			}
			cout << endl;
		}
		cout << "Es ronda: " << ronda( personas, n ) << endl;
	}
	
	return 0;
}

