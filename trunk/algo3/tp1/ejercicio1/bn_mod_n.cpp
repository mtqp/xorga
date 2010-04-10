#include <iostream>
#include <iomanip>
#include "../medicion.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

ullint operaciones = 0;

ullint bn_mod_n( uint b, ullint n )
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
	uint b, r;
	ullint n;
	uint ts=0;

	while(cin >> b && b!=(uint)-1){
		cin >> n;
		if(argc>1 && string(argv[1])=="time"){

			empezar_medicion(ts);
			r = bn_mod_n(b,n);
			terminar_medicion(ts);

			cout << setw(4) << b << "^" << setw(4) << n << " mod " << setw(4) << n << " = " << setw(4) << r;
			cout << "\t[" << setw(2) << ts << "c," << setw(3) << contador << "ops]" << endl;
		}
		else cout << bn_mod_n(b,n) << endl;
	}
	
	return 0;
}

