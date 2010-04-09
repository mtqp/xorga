#include<cmath>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<list>

#include<cstdlib>
#include<string>

#include "auxiliares.h"

using namespace std;

void random(string file, int cant_test, int max_amigas){

	cout << "argv3 == " << cant_test << "ESTAN DANDO SARASASASASA" << endl;
	cout << "argv4 == " << max_amigas << endl;
	
	srand(time(NULL));
	const string file_in = file + ".in";
	
	cout << file_in << endl;

	ofstream f;
	ofstream f_out;
	f.open("random.in");	//YO QUIERO Q SEA EL FILE_IN!!!! PERO NO ME ANDA
	f_out.open("random.out");
	
	int i = 0;
	int r;
	int cant_amigas;
	for (i;i<cant_test;i++){
		r = rand()%2;
		cant_amigas = rand()%(max_amigas+1);
		list<int> array_amigas[cant_amigas];
		cant_amigas = rand()%(max_amigas+1); 	//setear cant amigas mayor a tres x precond MAN
		if(cant_amigas < 3) cant_amigas = 3;
		if (r) {
			//ronda_no_posible(array_amigas, cant_amigas);
			f_out << "no" << endl;
		} else {
			cant_amigas = rand()%(max_amigas+1);
			ronda_posible(array_amigas, cant_amigas);
			f_out << "ronda" << endl;
			guardar_ronda(f, array_amigas, cant_amigas);
		}
//		guardar_ronda(f, array_amigas, cant_amigas);
	}
	f << "-1" << endl;
	f_out.close();
	f.close();
}


void rondas(string file, int cant_test, int max_amigas){
	srand(time(NULL));
	const string file_in = file + ".in";
	
	cout << file_in << endl;

	ofstream f;
	ofstream f_out;
	f.open("rondas.in");
	f_out.open("rondas.out");
	
	int i = 0;
	int r;
	int cant_amigas;
	for (i;i<cant_test;i++){
		cant_amigas = rand()%(max_amigas+1);
		list<int> array_amigas[cant_amigas];
		cant_amigas = rand()%(max_amigas+1);
		ronda_posible(array_amigas, cant_amigas);
		f_out << "ronda" << endl;
		guardar_ronda(f, array_amigas, cant_amigas);
	}
	
	f << "-1" << endl;
	f_out.close();
	f.close();

}

/*void no_rondas(string file, int cant_test, int max_amigas){
	srand(time(NULL));
	const string file_in = file + ".in";
	
	cout << file_in << endl;

	ofstream f;
	ofstream f_out;
	f.open("rondas.in");
	f_out.open("rondas.out");
	
	int i = 0;
	int r;
	int cant_amigas;
	for (i;i<cant_test;i++){
		cant_amigas = rand()%(max_amigas+1);
		list<int> array_amigas[cant_amigas];
		cant_amigas = rand()%(max_amigas+1);
		no_ronda_posible(array_amigas, cant_amigas);
		f_out << "ronda" << endl;
	}
	
	guardar_ronda(f, array_amigas, cant_amigas);
	
	f << "-1" << endl;
	f_out.close();
	f.close();

}*/
