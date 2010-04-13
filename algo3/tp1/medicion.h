#ifndef __MEDICION_H__
#define __MEDICION_H__

#define empezar_medicion(var) __asm__ __volatile__ ("rdtsc;mov %%eax, %0" : : "g" (var) )
#define terminar_medicion(var) __asm__ __volatile__ ("rdtsc;sub %0,%%eax;mov %%eax,%0" : : "g" (var) )

/*
#include<sys/time.h>

#define empezar_medicion(var) \
	gettimeofday(&tiempo, NULL);  \
	var = tiempo.tv_usec

#define terminar_medicion(var) \
	gettimeofday(&tiempo, NULL); \
	var = tiempo.tv_usec - var

timeval tiempo;
*/

#define O(n) contador+=(n)
#define reiniciar_contador() contador=0

/* contador de la cantidad de operaciones */
unsigned long long contador;

#endif

