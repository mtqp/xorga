#include "ciudad.h"
#include "../medicion.h"

int main (int argc, char** argv){
	ullint n;
	double ts;
	uint cantidad_de_calles;
	uint esquina;
	while(cin >> n && n!=(ullint)-1){
		uint** conexiones;
		conexiones = new uint* [n];
		for(uint i=0; i<n; i++){
			conexiones[i] = new uint [n];
			for(uint j=0; j<n; j++) conexiones[i][j]=0;
		}
		for(uint i=0; i<n; i++){
			cin >> cantidad_de_calles;
			for(uint j=0; j<cantidad_de_calles; j++){
				cin >> esquina;
				conexiones[i][esquina-1]=1;
			}
		}
		if(argc>1 && string(argv[1])=="time"){	//si el argumento es "time", mido el tiempo
			ts=0;
			medir_tiempo( ts, ciudad(conexiones,n), 1, 0.5f);
			cout << n << "\t" << ts << endl;
		}
		else if(argc>1 && string(argv[1])=="count"){	//si el argumento es "count", cuento cantidad de operaciones
			ciudad(conexiones,n);
			cout << n << "\t" << contador << endl;	//imprimo la cuenta
		}
		else{
			if(ciudad(conexiones,n)) cout << "fuertemente conexo" << endl;
			else cout << "no" << endl;
		}
		for(uint i=0; i<n; i++){	//libero la memoria
			delete [] conexiones[i];
		}
		delete [] conexiones;
	}
	return 0;
}

bool ciudad(uint** conexiones, uint n) {
	/*crea array de nodos pertenecientes al ciclo*/
	bool nodos_ciclo [n];
	limpiar_array(nodos_ciclo, n);

	uint cant_nodos_ciclo = 0;

	uint nodo_salida;
	bool  encontre_ciclo;

	encontre_ciclo = dfs_busco_primer_ciclo(conexiones, nodos_ciclo, n); 	//O(n²)
	cant_nodos_ciclo = nodos_en_ciclo(nodos_ciclo,n);											//O(n)

	while((cant_nodos_ciclo < n) && encontre_ciclo){	//O(n³)
		if(dame_arista_libre_ciclo(nodo_salida, conexiones, nodos_ciclo, n)){					//O(n²)
			encontre_ciclo = formar_ciclo_desde(nodo_salida, conexiones, nodos_ciclo, cant_nodos_ciclo, n); //O(n²)
		}
		else{
			encontre_ciclo = false;
		}
		reseteo_matriz_salvo_ciclo(conexiones, nodos_ciclo,n);			//O(n²)
	}
	return encontre_ciclo;
}

bool dfs_busco_primer_ciclo(uint** conexiones, bool* nodos_ciclo, uint n){
	pila  p;
	lista l;
	lista l_ady;
	bool nodos_ciclo_posible[n];

	bool encontre_ciclo = false;
	bool tiene_algo_para_agregar		= true;
	uint nodo_actual;
	
	p.push(0);									//push 1er nodo (EMPIRICAMENTE, el 0)			
	nodos_ciclo_posible[0] = true;	
	
	while((!encontre_ciclo) && tiene_algo_para_agregar){
		nodo_actual = p.top();
		l.push_back(nodo_actual);
		p.pop();
		tiene_algo_para_agregar = dame_adyacentes_a(nodo_actual,conexiones, l_ady, n);
		
		while((!l_ady.empty()) && tiene_algo_para_agregar && (!encontre_ciclo)){
			if(!nodos_ciclo_posible[l_ady.front()]){
				p.push(l_ady.front());
				l_ady.pop_front();
				nodos_ciclo_posible[p.top()] = true;
			}else{
				l.push_back(l_ady.front());
				encontre_ciclo = true;
			}
		}
	}
	if(!tiene_algo_para_agregar) return no_fuertemente_conexo;
	if(encontre_ciclo){
		buscar_y_marcar_ciclo(conexiones, nodos_ciclo, /*nodos_ciclo_posibles, */l, n);
	}
	
	limpiar_array(nodos_ciclo_posible,n);
	return encontre_ciclo && tiene_algo_para_agregar;
}

bool formar_ciclo_desde(uint& nodo_salida, uint** conexiones, bool* nodos_ciclo, uint& cant_nodos_ciclo, uint n){
	pila  p;
	lista l;
	lista l_ady;

	bool volvi_al_ciclo	= false;
	bool bool_basura;
	uint nodo_actual;
	
	p.push(nodo_salida);			
	
	while((!p.empty())&&(!volvi_al_ciclo)){								//O(n²)
		nodo_actual = p.top();
		l.push_back(nodo_actual);
		p.pop();
		bool_basura = dame_adyacentes_a(nodo_actual,conexiones, l_ady, n);
		
		while((!l_ady.empty()) && (!volvi_al_ciclo)){
			if(nodos_ciclo[l_ady.front()])
				volvi_al_ciclo = true;
			else {
				p.push(l_ady.front());
				l_ady.pop_front();
			}		
		}
	}
	if(volvi_al_ciclo) {			//2*O(n)
		cant_nodos_ciclo += l.size();
		while(!l.empty()){			//O(n)
			nodos_ciclo[l.front()] = true;
			l.pop_front();
		}
		while(!l_ady.empty()){		//O(n)
			uint top = l_ady.front();
			l_ady.pop_front();
			if(!nodos_ciclo[top]){
				for(int i=0; i<n; i++){
					if(conexiones[top][i] == 2){
						conexiones[top][i] = 1;
						conexiones[i][top] = 1;
					}
				}
			}
		}
		return true;
	}else {
		return no_fuertemente_conexo;
	}
}

bool dame_arista_libre_ciclo(uint &nodo_salida, uint** conexiones, bool* nodos_ciclo, uint n){
	bool arista_libre = false;
	for(int i=0; (i<n)&&(!arista_libre) ;i++){
		if(nodos_ciclo[i]){
			for(int j=0; (j<n)&& (!arista_libre); j++){
				if(conexiones[i][j] == 1){
					conexiones[i][j] = 2;
					conexiones[j][i] = 2;
					arista_libre = true;
					nodo_salida	 = j;	
				}
			}
		}
	}
	return arista_libre;
}

bool dame_adyacentes_a(uint nodo_actual,uint** conexiones, lista &l_ady, uint n){
	for(int j=0; j<n; j++){
		if(conexiones[nodo_actual][j] == 1){
			conexiones[nodo_actual][j] = 2;
			conexiones[j][nodo_actual] = 2;
			l_ady.push_back(j);
		}
	}
	return !l_ady.empty();
}

void reseteo_matriz_salvo_ciclo(uint** conexiones, bool *nodos_ciclos, uint n){
	for(int i=0; i<n; i++){
		if(nodos_ciclos[i]==0){
			for(int j=0;j<n;j++){
				if(conexiones[i][j] == 2){ 
					conexiones[i][j]=1;
					conexiones[j][i]=1;
				}
			}
		}
	}
}

void buscar_y_marcar_ciclo(uint** conexiones, bool* nodos_ciclos, lista &l, uint n){
	bool encontre_ciclo = false;
	uint ultimo_int = l.back();
	uint primer_int;
	
	while(!encontre_ciclo){
		primer_int = l.front();
		if(conexiones[ultimo_int][primer_int] == 2)
			encontre_ciclo = true;
		else
			l.pop_front();
	}
	while(!l.empty()){
		nodos_ciclos[l.front()] = true;
		l.pop_front();
	}
	reseteo_matriz_salvo_ciclo(conexiones, nodos_ciclos, n);
}

uint nodos_en_ciclo(bool* nodos, uint n){
	uint res=0;
	for(int j=0; j<n; j++){
		if(nodos[j]) res++;
	}
	return res;
}
