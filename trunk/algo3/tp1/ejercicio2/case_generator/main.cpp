#include<cmath>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>

#include<cstdlib>

#include "case_generator.h"

using namespace std;

int main (int argc, char** argv){
	bool modo_generacion = false;
	if(argc<5 || argc <= 1) {
		cout << "Ingrese en el siguiente orden:" << endl;
		cout << "	Modo de generacion" << endl;
		cout << "	Nombre de los archivos de salida" << endl;
		cout << "	Cantidad de test a generar" << endl;
		cout << "	Cantidad maxima de amigas por test" << endl << endl;
		cout << "Modos posibles  de generacion: '-random', '-rondas' y '-no_rondas'" << endl;
	} else {
		int cant_test  = atoi(argv[3]); 
		int cant_amigas= atoi(argv[4]);
		string option = string(argv[1]);
		bool cant_test_valida = true;
		bool cant_amigas_valida = true;
		
		
		if (cant_test <= 0){
			cout << "Elija una cantidad de test mayor a cero para realizar" << endl;
			cant_test_valida = false;
		}
		
		if(cant_amigas < 3){
			cout << "Elija una cantidad de amigas mayor a dos para generar tests" << endl;
			cant_amigas_valida = false;
		}
		
		if((argc > 1) && cant_test_valida & cant_amigas_valida){
			if (option=="-random"){
				cout << "Generando en modo 'random'" << endl;
				random(argv[2], cant_test, cant_amigas);	
				modo_generacion = true;
			}
			if(option == "-rondas"){
				cout << "Generando en modo 'rondas'" << endl;
				rondas(argv[2], cant_test, cant_amigas);
				modo_generacion = true;
			}
			if(option == "-no_rondas"){		
				if(cant_amigas < 5) {
					cout << "Imposible generar una no-ronda con menos de cinco amigas" << endl;
				} else {
				cout << "Generando en modo 'no_rondas'" << endl;
				no_rondas(argv[2], cant_test, cant_amigas);
				modo_generacion = true;
				}
			} 
			
			if (modo_generacion){
				cout << "Las entradas generadas se guardaron en '" << argv[2];
				cout << ".in' y las salidas esperadas en '" << argv[2] << ".out'" << endl;
			} else {
				cout << "Modos posibles  de generacion: '-random', '-rondas' y '-no_rondas'" << endl;
				//modo_generacion = false;
			}
		}
	}
	return 0;
}
