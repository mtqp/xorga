#ifndef __AUXILIARES_H__
#define __AUXILIARES_H__

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<list>

#include<cstdlib>

using namespace std;

void guardar_ronda(ofstream &f, list<int> *array_amigas, int cant_amigas);
void agregar_ronda(list<int> *array_amigas, int *orden, int cant);
void ronda_posible(list<int> *array_amigas, int cant_amigas);
bool no_repita_amigas(list<int> lista, int size);
bool shuffle_completo(bool *a_bools, int size);

#endif
