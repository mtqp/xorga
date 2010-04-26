typedef unsigned int uint;
typedef unsigned long long int ullint;

typedef struct _Nodo_lista{
	/*
		invariante de arista: pi1 < pi2 SIEMPRE
	*/
	uint _pi1;
	uint _pi2;
	Nodo_lista* _siguiente_lista;
	Nodo_lista* _anterior_lista;
}Nodo_lista;

typedef struct _Lista {
	Nodo_lista * _primero_lista;
	Nodo_lista * _ultimo_lista;
}Lista;

typedef struct _Nodo_matriz {
	/*
		invariante de nodo matriz: 
			if(_adyacente) 	ptr_arista != NULL
			else 			ptr_arista == NULL
	*/
	int _adyacente;				// es 1 o 0, tipo bool
	int _numero_circuito;
	Nodo_lista *_ptr_arista;	
}Nodo_matriz;

