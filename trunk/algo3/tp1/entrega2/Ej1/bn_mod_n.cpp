#include <iostream>
#include <iomanip>
#include "../medicion.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

ullint operaciones = 0;

ullint bn_mod_n( ullint b, ullint n )
{
	reiniciar_contador();

	ullint tmp=1;
	ullint m = n;
	b = b%n;

	// caso base: b < 2 => b mod X = b
	O(5);
	if( b < 2 ) 
		return b;

	while( n > 0 )
	{
		if( n%2 == 1 )
		{
			tmp *= b;
			tmp %= m;
			O(4);
		}
		n /= 2;
		b *= b;
		b %= m;
		O(9);
	}	

	return tmp;
}

int main (int argc, char** argv)
{
	uint b;
	ullint n;
	double ts;

	while(cin >> b && b!=(uint)-1){
		cin >> n;
		if(argc>1 && string(argv[1])=="time"){
			//empezar_medicion(ts);
			medir_tiempo( ts, bn_mod_n(b,n), 1, 0.5f );
			//terminar_medicion(ts);
			cout << n << "\t" << ts << endl;
		}
		else if( argc > 1 && string(argv[1])=="count")
		{
			bn_mod_n(b,n);
			cout << n << "\t" << contador << endl;
		}
		else cout << bn_mod_n(b,n) << endl;
	}
	
	return 0;
}

