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

void agregar_ronda(list<int> *array_amigas, int *orden, int cant){
	int i = 1;
	array_amigas[orden[0]].push_back(orden[cant-1]);
	for(i;i<cant;i++){
		array_amigas[orden[i]].push_back(orden[i-1]);
	}//convencerse q nunca agregar en amigas[i] efectivamente a i;
}

bool no_repita_amigas(list<int> lista, int iesimo, int size){
	list<int> :: iterator it;
	int cant_veces[size];
	bool no_repite = true;
	int z = 0;
	for ( it=lista.begin();it!=lista.end(); it++ ){
		cant_veces[*it]++;
	}

	while(z<size && no_repite){
		no_repite = cant_veces[z]<2;
	}
	return no_repite && (cant_veces[iesimo]==0);
}

void ronda_posible(list<int> *array_amigas, int cant_amigas){
	bool generar_ronda[cant_amigas];
	int ronda_amigas[cant_amigas];
	int amigas[cant_amigas];
	int i;
	int j;
	int index_vacio;
	
	srand(time(NULL));
		
	for(i=0;i<cant_amigas;i++){
		generar_ronda[i] = false;
		amigas[i] = i;
	}
	i = 0;
	while(!shuffle_completo(generar_ronda,cant_amigas)){
		index_vacio = rand()%cant_amigas;
		if(!generar_ronda[index_vacio]){
			generar_ronda[index_vacio] = true;
			ronda_amigas[i] = amigas[index_vacio];
			i++;
		}
	}
	////////recien aca tengo en ronda_amigas una solucion ahora tengo q completar con mas relaciones
	///x el momento agrego la ronda posible como ultimo nodo de array amigas podria despues hacerse un rand 
	/// entre agregar adelante y agregar atras, no es tan dificil... usar un if mod2
	agregar_ronda(array_amigas, ronda_amigas, cant_amigas);
	
	int cant_am_rand;
	int am_rand;
	int am_camino;
	for(i=0;i<cant_amigas;i++){
		cant_am_rand = rand()%cant_amigas;				//no puede ser amiga de si misma
		j=0;
		while(j<cant_am_rand){
			am_rand = rand()%cant_amigas;
			am_rand--;
			if (no_repita_amigas(array_amigas[i],i,cant_amigas)){	//q no se quiera hacer amiga de si misma o de la q ya era amiga
				if (rand()%2){
					array_amigas[i].push_front(am_rand);
				} else {
					array_amigas[i].push_back(am_rand);
				}
				j++;
			}
		}
	}
}

