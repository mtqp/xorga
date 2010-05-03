#ifndef __MEDICION_H__
#define __MEDICION_H__

#include <time.h>

#define medir_tiempo(_var,_funcion,_intentos,_tiempo) \
	clock_t _start, _end; \
	int _i; \
	double _t; \
	for( _i = 0 ; _i < _intentos ; _i++ ) \
	{ \
		_start = clock(); \
		long long _c = 0; \
		while( (double)(clock()-_start)/(CLOCKS_PER_SEC) < _tiempo ) \
		{ \
			_funcion;\
			_c++; \
		} \
		_end = clock(); \
		_t = (double)(_end-_start)/(CLOCKS_PER_SEC*_c); \
	} \
	_var = _t


#define O(n) contador+=(n)
#define reiniciar_contador() contador=0

/* contador de la cantidad de operaciones */
unsigned long long contador;

#endif

