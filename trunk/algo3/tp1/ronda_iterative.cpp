#include<list>
#include<iostream>

using namespace std;

typedef list<int> ListaDeAmigas;

////////////////////////////FUNCIONES AUXILIARES/////////////////////
void imprimir_resultado(ListaDeAmigas :: iterator *solucion, int longitud_solucion){
//	cout << "entrando a imprimi resultado" << endl;
	int i = 0;
	cout << "Existe solucion, el orden de amigas encontrado es el siguiente:" << endl;
	for(i;i<longitud_solucion;i++){
		cout << *solucion[i] << endl;
	}
//	cout << "saliendo imprimir resultado" << endl;
}

bool ronda_posible(ListaDeAmigas :: iterator *solucion, int longitud_solucion){
//	cout << "entrando ronda posible" << endl;
	return *solucion[longitud_solucion-1] == 0;
//	cout << "saliendo ronda posible" << endl;
}
	
bool dame_ult_it_not_NULL(ListaDeAmigas :: iterator *solucion, ListaDeAmigas *personas, int retroceder_hasta){
//	cout << "entrando dame ult is not null" << endl;
	bool encontre_camino = false;
	while (!encontre_camino){
		/*en solucion[i-1] tengo el iterador de la chica i-1 a sus amigas
		**tengo q ver si ese iterador no es nulo, si es sigo subiendo
		**si no, modifico solucion[i-1] con el iterador siguiente y devuelvo true*/
		retroceder_hasta--;

		if (retroceder_hasta == -1){		//me pase y se me acabo el arreglo
			return true;
		}

		solucion[retroceder_hasta]++;
		if (retroceder_hasta == 0){
			if(solucion[retroceder_hasta] != personas[0].end()){
				return false;
			}
		}else {
			if(solucion[retroceder_hasta] != personas[*solucion[retroceder_hasta-1]].end()){
				return false;
			}
		}
	}
//	cout << "saliendo ult is not null" << endl;
}

bool menorQDosAmigas (ListaDeAmigas *personas, int n){
//	cout << "entrando menor q dos amigas" << endl;
	int i = 0;
	bool res = true;
	for (i; (i<n) && res;i++) {
		res = res && (personas[i].size() <= 1);
	}
//	cout << "saliendo menor q dos amigas" << endl;
	return res;
}

bool algunaAmigaRepetida(ListaDeAmigas::iterator *solucion, int longitud){
//	cout << "entrnado aluna amiga repetida" << endl;
	/*se puede optimizar xq de precond son >= a tres 
	**la cant chicas y no son amigas de si mismas*/
	int i = 0;
	bool repetida = false;
	for(i;i<longitud && (!repetida);i++){
		repetida = (*(solucion[i]) == *(solucion[longitud])) || (0 == *solucion[i]);
		///recordemos q la amiga cero solo puede aparecer en el indice i-1 y nada mas q ahi
	}
//	cout << "saliendo alguna amiga repetida" << endl;
	return repetida;
}
//////////////////////////////////////////////////////////////////

bool ronda( ListaDeAmigas* personas, int n ){

	//cout << "entrando a ronda!!" << endl;
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
						
			///WHILE 2///
			while(!encontre_posible && !it_apunta_NULL){
				//----IF 1---------//
				if(i == longitud_solucion-1) {						//mi arreglo solucion esta lleno!
					//---------------//
					if (ronda_posible (solucion,longitud_solucion)){	//hay ronda? //SIEMPRE ESTA SIN REP SALVO ULT==1ro
						return true;
						cout << "saliendo de ronda" << endl;
						//imprimir_resultado(solucion,longitud_solucion); //imprime en pantalla el orden de las amigas
					} else {
						it_actual++;								//existe otro camino para recorrer o tengo q subir?
						if (it_actual != personas[i].end()){
							it_apunta_NULL = true;
						}
					}
					//---------------//
				} else {
					//---------------//
					if (!algunaAmigaRepetida(solucion, i)){			//la nueva chica no estaba previamente en la ronda?
						encontre_posible = true;
					} else {
						it_actual++;								//existe otro camino para recorrer o tengo q subir?
						if (it_actual != personas[i].end()){
							it_apunta_NULL = true;
						} else {
							encontre_posible = true;
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
				probe_todas_posibilidades = !dame_ult_it_not_NULL(solucion,personas,i);
			}
			
		//	cout << "encontre posible == " << encontre_posible << endl;
		//	cout << "it_apunta_NULL == "<< it_apunta_NULL << endl;
			encontre_posible= false;
			it_apunta_NULL 	= false;
		}
		//cout << "saliendo de ronda!!" << endl;
		return false;	
	}
	
}

int main( ) 
{
	int n;
	int a, cant_amigas;
	while(cin >> n && n!=-1){
		ListaDeAmigas personas[n];
		//bool p[n];
		cout << "Son " << n << " amigas" << endl;
		for( int i = 0 ; i < n ; i++ )
		{
			cin >> cant_amigas;
			cout << "La persona " << i << " tiene " << cant_amigas <<": ";
			//p[i]= false;
			for( int j = 0 ; j < cant_amigas ; j++ )
			{
				cin >> a;
				cout << a-1 << " ";
				personas[i].push_back( a-1 );
			}
			cout << endl;
		}
		//p[0]=true;
		cout << "Es ronda: " << ronda( personas, n ) << endl;
	}
	
	return 0;
}

