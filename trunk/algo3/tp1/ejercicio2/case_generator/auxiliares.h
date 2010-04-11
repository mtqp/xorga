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
void ronda_no_posible(list<int> *array_amigas, int cant_amigas);
bool no_repita_amigas(list<int> lista, /*int size, */int am_rand);
bool shuffle_completo(bool *a_bools, int size);
void shuffle(bool *generar_ronda, int *ronda_amigas, int *amigas, int cant_amigas);
void amigar(list<int> *array_amigas, int *shuf_amigas, int sub_A, int sub_B);
#endif
