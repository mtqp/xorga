#include<iostream>
#include<sys/time.h>
using namespace std;

#define empezar_medicion(tvar, var) \
	gettimeofday(&tvar, NULL);  \
	var = tvar.tv_usec

#define terminar_medicion(tvar, var) \
	gettimeofday(&tvar, NULL); \
	var = tvar.tv_usec - var

typedef unsigned int uint;
typedef unsigned long long int ullint;

uint bn_mod_n( uint b, ullint n )
{
	uint c = b%n;

	// caso base 1: b == 1 => resultado = 1
	if( b == 1 )
		return 1;
	
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

#include<iomanip>

int main (int argc, char** argv)
{
	uint b, r;
	ullint n;
	ullint ts;
	timeval tv;

	while(cin >> b && b!=-1){
		cin >> n;
		if(argc>1 && string(argv[1])=="time"){
			empezar_medicion(tv,ts);
			r = bn_mod_n(b,n);
			terminar_medicion(tv,ts);
			cout << setw(4) << b << "^" << setw(4) << n << " mod " << setw(4) << n << " = " << setw(4) << r;
			cout << "\t[" << ts << "µs]" << endl;
		}
		else cout << bn_mod_n(b,n) << endl;
	}
	return 0;
}

