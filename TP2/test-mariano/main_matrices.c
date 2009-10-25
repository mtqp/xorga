#include <stdlib.h>
#include <stdio.h>

extern void pm(char* ptr_src,char* ptr_dst);

int main(){
	srand(time(NULL));
	char * ptr_src = malloc(16);
	int i;
	for(i=0;i<16;i+=1){
		if (i<5) {
			ptr_src[i]=2^i;
		}
		if (i<10){
			ptr_src[i]=3*i;
		}
		else {
			ptr_src[i]=2*i;
		}
	}
	char * ptr_dst = malloc(16);
	pm(ptr_src,ptr_dst);
	for(i=0;i<16;i+=1){
		printf("elemento_%d es: %d\n",i,ptr_dst[i]);
	}
	return 0;
}

