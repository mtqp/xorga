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

	uint c = b%n;
	ullint tmp=1;
	ullint m = n;

	// caso base 1: n == 1 => X mod 1 = 0
	// caso base 2: c == 0 => 0 mod X = 0
	O(6);
	if( n == 1 || c == 0 ) 
		return 0;

	// caso base 3: c == 1 => resultado = 1
	O(1);
	if( c == 1 )
		return 1;

	while( m > 0 )
	{
		if( m%2 == 1 )
		{
			tmp *= c;
			tmp %= n;
			O(2);
		}
		m /= 2;
		c *= c;
		c %= n;
		O(6);
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

