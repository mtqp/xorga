#include <stdlib.h>
#include <stdio.h>
#include <math.h>
extern void asmFreiChen(char* ptr_src, float* ptr_dst,int witdh,int line_selector);

float cFreich(char * ptr_src,int width,int elem_i,int sqr){
	float res= 0.0;
	float raiz2= sqrt(2);
	if (sqr==0){
		if (elem_i == 0 || elem_i == width-1)
			return res=0;
		else
			res = (-1.0)*(float)(ptr_src[elem_i-1])+(float)(ptr_src[elem_i+1]);
		return res;}
	else {
		if (elem_i == 0 || elem_i == width-1)
			return res=0;
		else
			res = (-raiz2)*(float)(ptr_src[elem_i-1])+(raiz2*(float)(ptr_src[elem_i+1]));
		return res;
	}
}

int main(){
	srand(time(NULL));
	int width = 16;			//test como si tuviera solo dos lineas
	int i;
	int line_sqr = 1;		//quiero q calcule con raiz!!!
	char * ptr_src = (char*) malloc(width);
	float * ptr_dst = (float*) malloc(4*width);
	//printf("malloqueo ptr_dst y llamo a apply_lineRob... \n");
	//printf("si no tira error es porque son todos iguales en mil pasadas\n");
	int j;
	printf("10.000 iteraciones cmp q en C y ASM FREICH -101 funcionasen bien\n");
	printf("si no hay algun printf de error todas las cmp fueron exitosas\n");
	printf("test sin saturacion \n");
	for(j=0;j<10000;j+=1){				///menudo chequeo random para q ande!
		for(i=0;i<width;i+=1){
			ptr_src[i]=(char) (rand()%30); 
		}
				
		asmFreiChen(ptr_src,ptr_dst,width,line_sqr);
		for(i=0;i<(width-1);i+=1){
			//if((ptr_src[i]-ptr_src[17+i])>0){	///esta linea va x la saturacion si es menor van a ser != y explota
				if(ptr_dst[i] != (cFreich(ptr_src,width,i,line_sqr))) {
					printf("error FREICH_X -101  -> for_%d,dst_%d\n",j,i);
					printf("elemento_%d -> (dst = %f) == (Cres = %f)\n",i,ptr_dst[i], (cFreich(ptr_src,width,i,line_sqr)));
				}
			//}
		}
	}
	
		/*for(i=0;i<width;i+=1){
			ptr_src[i]=(char) (rand()%30); 
		}
		asmFreiChen(ptr_src,ptr_dst,width);
		for(i=0;i<(width);i+=1){
			if (i==0 || i==width-1)
			printf("elemento_%d -> (dst = %f) == (Cres = %f)\n",i,ptr_dst[i], 0.0);
			else 
			printf("elemento_%d -> (dst = %f) == (Cres = %f)\n",i,ptr_dst[i], cFreich(ptr_src,width,i));
		}
*/	
	//printf("esta bien q difiera el ultimo elem del src al del dest \n");	
	return 0;
}

