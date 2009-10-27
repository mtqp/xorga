#include <stdlib.h>
#include <stdio.h>

extern void apply_RobX(char* ptr_src, char* ptr_dst,int witdh);
extern void apply_RobY(char* ptr_src, char* ptr_dst,int witdh);

int main(){
	srand(time(NULL));
	int width = 16;			//test como si tuviera solo dos lineas
	int i;
	char * ptr_src = (char*) malloc(2*width+1);
	char * ptr_dst = (char*) malloc(16);
	//printf("malloqueo ptr_dst y llamo a apply_lineRob... \n");
	//printf("si no tira error es porque son todos iguales en mil pasadas\n");
	int j;
	printf("10.000 iteraciones cmp q en C y ASM robY y robX funcionasen bien\n");
	printf("si no hay algun printf de error todas las cmp fueron exitosas\n");
	printf("test sin saturacion \n");
	for(j=0;j<10000;j+=1){				///menudo chequeo random para q ande!
		for(i=0;i<16;i+=1){
			ptr_src[i]=(char) (rand()%30); 
		}
		for(i=16;i<(2*width);i+=1){
			ptr_src[i]=(char) (rand()%10);
		}
		apply_RobY(ptr_src,ptr_dst,width);
		for(i=0;i<(width-1);i+=1){
			//if((ptr_src[i+1]-ptr_src[16+i])>0){	///esta linea va x la saturacion si es menor van a ser != y explota
				if(ptr_dst[i] != (ptr_src[i+1]-ptr_src[16+i])) {
					printf("error ROB_Y for_%d,dst_%d\n",j,i);
					printf("elemento_%d -> (dst = %d) == (Cres = %d)\n",i,ptr_dst[i], (ptr_src[i+1]-ptr_src[16+i]));
				}
			//}
		}
				
		apply_RobX(ptr_src,ptr_dst,width);
		for(i=0;i<(width-1);i+=1){
			//if((ptr_src[i]-ptr_src[17+i])>0){	///esta linea va x la saturacion si es menor van a ser != y explota
				if(ptr_dst[i] != (ptr_src[i]-ptr_src[17+i])) {
					printf("error ROB_X for_%d,dst_%d\n",j,i);
					printf("elemento_%d -> (dst = %d) == (Cres = %d)\n",i,ptr_dst[i], (ptr_src[i]-ptr_src[17+i]));
				}
			//}
		}
	}
	/*
		for(i=0;i<16;i+=1){
			ptr_src[i]=(char) (rand()%30); 
		}
		for(i=16;i<(2*width);i+=1){
			ptr_src[i]=(char) (rand()%10);
		}
		apply_RobY(ptr_src,ptr_dst,width);
		//apply_RobX(ptr_src,ptr_dst,width);
		for(i=0;i<(width-1);i+=1){
			printf("elemento_%d -> (dst = %d) == (Cres = %d)\n",i,ptr_dst[i], (ptr_src[i+1]-ptr_src[16+i]));
		}
	*/
	//printf("esta bien q difiera el ultimo elem del src al del dest \n");	
	return 0;
}

