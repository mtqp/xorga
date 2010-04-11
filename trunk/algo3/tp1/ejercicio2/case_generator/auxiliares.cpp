#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<list>

#include<cstdlib>

using namespace std;

void guardar_ronda(ofstream &f, list<int> *array_amigas, int cant_amigas){
	int i = 0;
	f << cant_amigas << endl;
	for(i;i<cant_amigas;i++){
		f << array_amigas[i].size() << " ";
		while(!array_amigas[i].empty()){
			f << array_amigas[i].front()+1 << " ";
			array_amigas[i].pop_front();
		}
		f << endl;
	}
}

bool shuffle_completo(bool *a_bools, int size){
	bool alguno_falso = true;
	for(int i=0;i<size && (alguno_falso);i++){
		alguno_falso = a_bools[i];
	}
	return alguno_falso;
}

void shuffle(bool *generar_ronda, int *ronda_amigas, int *amigas, int cant_amigas){
	int i;
	int index_vacio;
	srand(time(NULL));
	
	for(i=0;i<cant_amigas;i++){
		generar_ronda[i] = false;
		amigas[i] = i;
	}
	
	i = 0;

	while(!shuffle_completo(generar_ronda,cant_amigas) || (i!=cant_amigas)){
		index_vacio = rand()%cant_amigas;
		if(!generar_ronda[index_vacio]){
			generar_ronda[index_vacio] = true;
			ronda_amigas[i] = amigas[index_vacio];
			i++;
		}
	}	
}

void agregar_ronda(list<int> *array_amigas, int *orden, int cant){
	int i = 1;
	array_amigas[orden[0]].push_back(orden[cant-1]);		//agrego las relaciones a izquierda
	for(i;i<cant;i++){
		array_amigas[orden[i]].push_back(orden[i-1]);
	}//convencerse q nunca agregar en amigas[i] efectivamente a i;

	i=0;
	array_amigas[orden[cant-1]].push_back(orden[0]);		//agrego las relaciones a derecha
	for(i;i<cant-1;i++){
		array_amigas[orden[i]].push_back(orden[i+1]);
	}
}

bool no_repita_amigas(list<int> lista,/* int size,*/ int am_rand){
	list<int> :: iterator it;
	bool res = true;

	for ( it=lista.begin();it!=lista.end(); it++ ){
		res = res && !(*it==am_rand);
	}
	return res;
}

void amigar(list<int> *array_amigas, int *shuf_amigas, int sub_A, int sub_B){
	array_amigas[shuf_amigas[sub_A]].push_back(shuf_amigas[sub_B]);
	array_amigas[shuf_amigas[sub_B]].push_back(shuf_amigas[sub_A]);
}

void ronda_posible(list<int> *array_amigas, int cant_amigas){

	bool generar_ronda[cant_amigas];
	int ronda_amigas[cant_amigas];
	int amigas[cant_amigas];
	int i;
	int j;
	
	srand(time(NULL));

	shuffle(generar_ronda, ronda_amigas, amigas, cant_amigas);
	////////recien aca tengo en ronda_amigas una solucion ahora tengo q completar con mas relaciones
	///x el momento agrego la ronda posible como ultimo nodo de array amigas podria despues hacerse un rand 
	/// entre agregar adelante y agregar atras, no es tan dificil... usar un if mod2
	agregar_ronda(array_amigas, ronda_amigas, cant_amigas);
	int cant_am_rand;
	int am_rand;
	int am_camino;
	bool puede_mas_amigas;

	for(i=0;i<cant_amigas;i++){
		cant_am_rand = rand()%cant_amigas-2;				//no puede ser amiga de si misma
		j=0;												//ni de las q ya es xq es camino
		puede_mas_amigas = true;

		while((j<cant_am_rand) && puede_mas_amigas){
			am_rand = rand()%cant_amigas;
			if (am_rand != i){
				puede_mas_amigas = (array_amigas[i].size()<cant_amigas-1);
				if (no_repita_amigas(array_amigas[i],am_rand)){	//q no se quiera hacer amiga de si misma o de la q ya era amiga
					if (rand()%2){
						array_amigas[i].push_front(am_rand);
						array_amigas[am_rand].push_front(i);
					} else {
						array_amigas[i].push_back(am_rand);
						array_amigas[am_rand].push_back(i);
					}
					j++;
				}
			}
		}
	}
}


void ronda_no_posible(list<int> *array_amigas, int cant_amigas){
	/*
		-hacer un shuffle de la entrada
		-partir la entrada en una cantidad random, pero posible para crear rondas
		-crear rondas con esos numeros para esas entradas
		-seleccionar randomizado un nodo de cada grafo "ronda" y unirlos entre si
		-devolver array amigas.
	*/

	bool generar_ronda[cant_amigas];
	int ronda_amigas[cant_amigas];
	int amigas[cant_amigas];
	int i;
	int j;
	
	srand(time(NULL));

	shuffle(generar_ronda, ronda_amigas, amigas, cant_amigas);
	/////////////GENERO DOS SUB RONDAS/////////////
	/*x cada array ahora, solo es posible que sean amigos esos numeros entre si*/

	if(cant_amigas == 5){
		amigar(array_amigas, ronda_amigas, 0, 1);
		amigar(array_amigas, ronda_amigas, 1, 2);
		amigar(array_amigas, ronda_amigas, 2, 0);
		
		amigar(array_amigas, ronda_amigas, 2, 3);
		amigar(array_amigas, ronda_amigas, 3, 4);
		amigar(array_amigas, ronda_amigas, 4, 2);	
		
	} else {

		int tam_sub_A = cant_amigas/2;
		int tam_sub_B = cant_amigas - tam_sub_A;
		int sub_array_A[tam_sub_A];
		int sub_array_B[tam_sub_B];

		for(i=0;i<tam_sub_A;i++){
			sub_array_A[i] = ronda_amigas[i];
		}
	
		for(i=0;i<tam_sub_B;i++){
			sub_array_B[i] = ronda_amigas[i+tam_sub_A];
		}

		////////////////////////////////////////////////
		/////GUARDO EN ARRAY AMIGAS LAS SUB RONDAS//////
		///////////////////////////////////////////////

		agregar_ronda(array_amigas, sub_array_A, tam_sub_A);
		agregar_ronda(array_amigas, sub_array_B, tam_sub_B);

		////////////////////////////////////////////////
		/////RANDOMIZAR AMIGAS PARA CADA SUB ARRAY//////
		////////////////////////////////////////////////
		int cant_am_rand;
		int am_rand;
		int am_camino;
		bool puede_mas_amigas;

		//ARRAY_A//
		for(i=0;i<tam_sub_A;i++){
			cant_am_rand = rand()%tam_sub_A-2;				//no puede ser amiga de si misma
			j=0;												//ni de las q ya es xq es camino
			puede_mas_amigas = true;

			while((j<cant_am_rand) && puede_mas_amigas){
				am_rand = sub_array_A[rand()%tam_sub_A];
				if (am_rand != sub_array_A[i]){
					puede_mas_amigas = (array_amigas[sub_array_A[i]].size()<tam_sub_A-1);
					if (no_repita_amigas(array_amigas[sub_array_A[i]],am_rand)){	//q no se quiera hacer amiga de si misma o de la q ya era amiga
						if (rand()%2){
							array_amigas[sub_array_A[i]].push_front(am_rand);
							array_amigas[am_rand].push_front(sub_array_A[i]);
						} else {
							array_amigas[sub_array_A[i]].push_back(am_rand);
							array_amigas[am_rand].push_back(sub_array_A[i]);
						}
						j++;
					}
				}
			}
		}
		//ARRAY_B//
		for(i=0;i<tam_sub_B;i++){
			cant_am_rand = rand()%tam_sub_B-2;				//no puede ser amiga de si misma
			j=0;												//ni de las q ya es xq es camino
			puede_mas_amigas = true;

			while((j<cant_am_rand) && puede_mas_amigas){
				am_rand = sub_array_B[rand()%tam_sub_B];
				if (am_rand != sub_array_B[i]){
					puede_mas_amigas = (array_amigas[sub_array_B[i]].size()<tam_sub_B-1);
					if (no_repita_amigas(array_amigas[sub_array_B[i]],am_rand)){	//q no se quiera hacer amiga de si misma o de la q ya era amiga
						if (rand()%2){
							array_amigas[sub_array_B[i]].push_front(am_rand);
							array_amigas[am_rand].push_front(sub_array_B[i]);
						} else {
							array_amigas[sub_array_B[i]].push_back(am_rand);
							array_amigas[am_rand].push_back(sub_array_B[i]);
						}
						j++;
					}
				}
			}
		}

		////ELIJO NODO QUE UNA AMBOS SUB ARRAYS////
		/*
			Notar que esta nodos, no son amigos antes, ya que las relaciones
			se generan para individuos dentro de un sub array
		*/
		int nodo_A = sub_array_A[rand()%tam_sub_A];
		int nodo_B = sub_array_B[rand()%tam_sub_B];
	
		array_amigas[nodo_A].push_front(nodo_B);
		array_amigas[nodo_B].push_front(nodo_A);
	}
}

