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
	bool modo_generacion;
	if(argc<5 || argc <= 1) {
		cout << "Ingrese en el siguiente orden:" << endl;
		cout << "	Modo de generacion" << endl;
		cout << "	Nombre de los archivos de salida" << endl;
		cout << "	Cantidad de test a generar" << endl;
		cout << "	Cantidad maxima de amigas por test" << endl << endl;
		cout << "Modos posibles  de generacion: '-random', '-rondas' y '-no_rondas'" << endl;
	} else {
		string option = string(argv[1]);
		if(argc > 1){
			if (option=="-random"){
				cout << "Generando en modo 'random'" << endl;
				random(argv[2], (int) *(argv[3]), (int) *(argv[4]));	
				modo_generacion = true;
				//esto no funciona bien pasando parametros!!!!!
			}
			if(option == "-rondas"){
				cout << "Generando en modo 'rondas'" << endl;
				rondas(argv[2], (int) *(argv[3]), (int) *(argv[4]));
				modo_generacion = true;
			}
			if(option == "-no_rondas"){		
				cout << "Generando en modo 'no_rondas'" << endl;
//				no_rondas(argv[2], (int) *(argv[3]), (int) *(argv[4]));
				modo_generacion = true;
			} else {
				cout << "Modos posibles  de generacion: '-random', '-rondas' y '-no_rondas'" << endl;
				modo_generacion = false;
			}
		
			if (modo_generacion){
				cout << "Las entradas generadas se guardaron en '" << argv[2];
				cout << ".in' y las salidas esperadas en '" << argv[2] << ".out'" << endl;
			}		
		}
	}
	return 0;
}
