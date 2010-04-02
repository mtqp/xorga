#include<cmath>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>

#define empezar_medicion(var) __asm__ __volatile__ ("rdtsc;mov %%eax, %0" : : "g" (var) );
#define terminar_medicion(var) __asm__ __volatile__ ("rdtsc;sub %0,%%eax;mov %%eax,%0" : : "g" (var) );

using namespace std;

typedef unsigned int uint;
typedef unsigned long long int ullint;

uint bn_mod_n_c( uint b, ullint n )
{
	return (ullint)pow( b, n )%n;
}

uint mod( ullint x, uint r )
{
	while( x >= r )
		x -= r;
	return x;
}

uint bn_mod_n( uint b, ullint n, ullint m )
{
	// caso base 1: b == 1 => resultado = 1
	if( b == 1 )
		return 1;
	
	// caso base 2: n == 1 => X mod 1 = 0
	if( n == 1 )
		return 0;
		
	uint c = mod( b, n );

	if( m == 1 )
		return c;

	// caso base 3: n == 2
	/*if( m == 2 )
		return mod(c * c, n);*/

	// caso recursivo
	uint tmp = bn_mod_n( c, n, m/2 );
	tmp *= tmp;

	// caso impar: 
	if( mod(m,2) == 1 )
		tmp *= c;

	return mod(tmp,n);
}

int main (int argc, char** argv)
{
	uint b;
	ullint n;
	ullint ts;
	while(cin >> b && b!=-1){
		cin >> n;
		if(argc>1 && string(argv[1])=="time"){
			empezar_medicion(ts);
			cout << b << "^" << n << " mod " << n << " = " << bn_mod_n(b,n,n);
			cout << "\t\t[" << ts << "]" << endl;
			terminar_medicion(ts);
		}
		else cout << bn_mod_n(b,n,n) << endl;
	}
	return 0;
}
