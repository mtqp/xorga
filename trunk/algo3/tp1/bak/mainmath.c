#include<math.h>
#include<stdio.h>
#include<time.h>

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
	if( m == 2 )
		return mod(c * c, n);

	// caso recursivo
	uint tmp = bn_mod_n( c, n, m/2 );
	tmp *= tmp;

	// caso impar: 
	if( mod(m,2) == 1 )
		tmp *= c;

	return mod(tmp,n);
}


int main ( )
{
	srand(time(NULL));
	
	uint b = rand()%10+1;
	ullint n = rand()%40+1;
	ullint veces = 10000000;
	
	ullint i;
	ullint tscl=0;
	
	__asm__ __volatile__ ("rdtsc;mov %%eax, %0" : : "g" (tscl) );
	for( i = 0; i < veces ; i++ )
		bn_mod_n_c(b, n);
	__asm__ __volatile__ ("rdtsc;sub %0,%%eax;mov %%eax,%0" : : "g" (tscl) );
	printf( "el de C: \t\t %Ld\n", tscl );

	__asm__ __volatile__ ("rdtsc;mov %%eax, %0" : : "g" (tscl) );
	for( i = 0; i < veces ; i++ )
		bn_mod_n(b, n, n);
	__asm__ __volatile__ ("rdtsc;sub %0,%%eax;mov %%eax,%0" : : "g" (tscl) );

	printf( "el nuestro: \t\t %Ld\n", tscl );
	
	printf( "C: %u^%Ld mod %Ld = %d\n", b, n, n, bn_mod_n_c( b, n ) );
	printf( "N: %u^%Ld mod %Ld = %d\n", b, n, n, bn_mod_n( b, n, n ) );
	
	//printf( "%d\n", mod(81,2) );
	return 0;
}

