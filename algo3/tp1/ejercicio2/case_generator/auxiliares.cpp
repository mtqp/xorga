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
	cout << "agregar ronda linea 33" << endl;		
	/*int i=0;
	cout << endl << "arreglo ORDEN == ";
	for(i;i<cant;i++){
		cout << orden[i] << " ";
	}
	cout << endl << endl;
	*/
	int i = 1;
	array_amigas[orden[0]].push_back(orden[cant-1]);		//agrego las relaciones a izquierda
	cout << "agregar ronda linea 36" << endl;		
	for(i;i<cant;i++){
		array_amigas[orden[i]].push_back(orden[i-1]);
		cout << "agregar ronda linea 39 con i==" << i << endl;		
	}//convencerse q nunca agregar en amigas[i] efectivamente a i;

	i=0;
	array_amigas[orden[cant-1]].push_back(orden[0]);		//agrego las relaciones a derecha
	for(i;i<cant-1;i++){
		array_amigas[orden[i]].push_back(orden[i+1]);
	}
	cout << "agregar ronda linea 41" << endl;		
}

bool no_repita_amigas(list<int> lista,/* int size,*/ int am_rand){
	//cout << "no repita amigas linea 52" << endl;
	list<int> :: iterator it;
	bool res = true;

	//cout << "no repita amigas linea 57" << endl;
	for ( it=lista.begin();it!=lista.end(); it++ ){
		res = res && !(*it==am_rand);
	}
	return res;
}

void ronda_posible(list<int> *array_amigas, int cant_amigas){
	cout << endl << "funcion ronda posible en aux.cpp linea 56" << endl;
	bool generar_ronda[cant_amigas];
	int ronda_amigas[cant_amigas];
	int amigas[cant_amigas];
	int i;
	int j;
	int index_vacio;
	
	srand(time(NULL));
	cout << "ronda posible linea 65" << endl;	
	for(i=0;i<cant_amigas;i++){
		generar_ronda[i] = false;
		amigas[i] = i;
	}
	i = 0;
//	cout << "sera cant amigas random??? == " << cant_amigas << endl;
	cout << "ronda posible linea 81 " << endl;	
	while(!shuffle_completo(generar_ronda,cant_amigas) || (i!=cant_amigas)){
//		cout << "ronda posible linea 83 con i==" << i << endl;	
		index_vacio = rand()%cant_amigas;
		if(!generar_ronda[index_vacio]){
			generar_ronda[index_vacio] = true;
			ronda_amigas[i] = amigas[index_vacio];
			i++;
		}
	}
	cout << "ronda posible linea 91" << endl;	
	////////recien aca tengo en ronda_amigas una solucion ahora tengo q completar con mas relaciones
	///x el momento agrego la ronda posible como ultimo nodo de array amigas podria despues hacerse un rand 
	/// entre agregar adelante y agregar atras, no es tan dificil... usar un if mod2
	agregar_ronda(array_amigas, ronda_amigas, cant_amigas);
	cout << "ronda posible linea 98" << endl;		
	int cant_am_rand;
	int am_rand;
	int am_camino;
	for(i=0;i<cant_amigas;i++){
		cout << "ronda posible linea 103 con i==" << i << endl;
		cant_am_rand = rand()%cant_amigas-2;				//no puede ser amiga de si misma
		j=0;												//ni de las q ya es xq es camino
		cout << "ronda posible linea 106 cant amigas random == " << cant_am_rand << endl;
		while(j<cant_am_rand){
			cout << "ronda posible linea 108 j==" << j << endl;		
			am_rand = rand()%cant_amigas;
			if (am_rand != i){
				if (no_repita_amigas(array_amigas[i],/*cant_amigas,*/am_rand)){	//q no se quiera hacer amiga de si misma o de la q ya era amiga
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
	cout << "fin ronda posible linea 103" << endl;
}

