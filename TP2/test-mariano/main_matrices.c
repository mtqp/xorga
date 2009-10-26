#include <stdlib.h>
#include <stdio.h>

extern void apply_lineX(char* ptr_src,int selector_line);
extern void save_mem(char* ptr_dst);

int main(){
	srand(time(NULL));
	char * ptr_src = malloc(16);
	ptr_src[0]= 10;
	ptr_src[1]= 33;
	ptr_src[2]= 24;
	ptr_src[3]= 4;
	ptr_src[4]= 67;
	ptr_src[5]= 54;
	ptr_src[6]= 34;
	ptr_src[7]= 22;
	ptr_src[8]= 9;
	ptr_src[9]= 80;
	ptr_src[10]= 5;
	ptr_src[11]= 43;
	ptr_src[12]= 2;
	ptr_src[13]= 3;
	ptr_src[14]= 1;
	ptr_src[15]=0;
	int i;
/*	for(i=0;i<16;i+=1){
		if (i<5) {
			ptr_src[i]=2^i;
		}
		if (i<10){
			ptr_src[i]=3*i;
		}
		else {
			ptr_src[i]=2*i;
		}
	}*/
	char * ptr_dst = malloc(16);
	apply_lineX(ptr_src,0);
	printf("paso el apply \n");
	save_mem(ptr_dst);
	for(i=0;i<16;i+=1){
		printf("elemento_%d es: %d\n",i,ptr_dst[i]);
	}
	return 0;
}

