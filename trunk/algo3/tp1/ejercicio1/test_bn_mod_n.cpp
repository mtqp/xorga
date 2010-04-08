#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<iomanip>

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

uint bn_mod_n( uint b, ullint n, ullint m )
{
	// caso base 1: b == 1 => resultado = 1
	if( b == 1 )
		return 1;
	
	// caso base 2: n == 1 => X mod 1 = 0
	if( n == 1 )
		return 0;
		
	uint c = b%n;

	if( m == 1 )
		return c;

	// caso base 3: n == 2
	/*if( m == 2 )
		return mod(c * c, n);*/

	// caso recursivo
	uint tmp = bn_mod_n( c, n, m/2 );
	tmp *= tmp;

	// caso impar: 
	if( m%2 == 1 )
		tmp *= c;

	return tmp%n;
}

uint _bn_mod_n( uint b, ullint n )
{
	uint c = b%n;

	// caso base 1: b = 0  ó b = 1 => resultado = b
	if( b < 2 )
		return b;
	
	// caso base 2: n == 1 => X mod 1 = 0
	if( n == 1 )
		return 0;

	ullint m   = n%2;
	ullint csq = c*c;
	ullint tmp;
	if( m == 1 )
		tmp = c;
	else
		tmp = 1;

	while( m < n )
	{
		tmp *= csq;
		tmp %= n;
		m += 2;
	}

	return tmp;
}


int main (int argc, char** argv)
{
	/**
	 * TODO: pensar casos de test no random, o random con resultado predecible
	 * es decir, buscar una manera de generar casos que ya sepa que van a dar
	 * por ejemplo 1, para después hacer el test y probar que es correcto.
	 * 
	 * De última si no se nos ocurre nada podemos hacer este mismo generador de
	 * test random en haskell o python y generar un .in y .out y que el programa
	 * checkee que da lo mismo que esos resultados, pero sería mejor lo primero.
	 * 
	 * Esta versión sólo verifica que funcione bien el bn_mod_n iterativo respecto
	 * al recursivo (que funcionaba bien)
	 */

	uint b;
	ullint n;
	ullint ts;
	srand(time(NULL));

	uint i;
	uint count = 20;
	if( argc == 2 )
		count = atoi(argv[1]);

	cout << setfill(' ');
	for( i = 0 ; i < count ; i++ )
	{
		b = rand()%1000;
		n = rand()%999+1;
		uint r = _bn_mod_n(b,n);
		cout << "TEST: " << setw(3) << b << "^" << setw(3) << n << " mod " << setw(3) << n << " = " << setw(3) << _bn_mod_n(b,n) << " ";
		if( r==bn_mod_n(b,n,n) )
			cout << "\33[32m" << "OK" << "\33[0m" << endl;
		else {
			cout << "\33[31m" << "FAIL" << "\33[0m" << endl;
			break;
		}
	}
	cout << i << " casos probados." << endl;
	return 0;
}

