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

	// caso recursivo
	uint tmp = bn_mod_n( c, n, m/2 );
	tmp *= tmp;

	// caso impar: 
	if( mod(m,2) == 1 )
		tmp *= c;

	return mod(tmp,n);
}

typedef string (PROCFUNC)( istringstream&, bool& );

bool procesarArchivo( const string& input_name, const string& output_name, PROCFUNC& callback )
{
	bool input_end = false;
	string resultado;
	ifstream input_file;
	ofstream output_file;
	string line;
	istringstream sstream;

	input_file.open(input_name.data());
	output_file.open(output_name.data());

	if( input_file.is_open( ) && output_file.is_open( ) )
	{
		while( !input_file.eof() && !input_end )
		{
			getline( input_file, line );
			sstream.str( line );
			resultado = callback( sstream, input_end );
			sstream.clear();
			if( !input_end )
				output_file << resultado << endl;
		}
	}
	else {
		cout << "No se pudo abrir el archivo de entrada" << endl;
		return false;
	}
	if( !input_end )
		cout << "Advertencia: archivo de entrada sin final." << endl;
	input_file.close();
	output_file.close();
	return true;
}

string procesar_input( istringstream& s, bool& end_of_input )
{
	uint result;
	ullint ts;
	int b, n;

	/**
	 * carga las variables b y n
	 **/
	s >> b; 
	s >> n;

	/**
	 * si es el final del archivo (-1,-1) no procesa la línea
	 **/
	end_of_input = ( b == -1 && n == -1 );
	if( end_of_input ) return "";

	/**
	 * toma el clock antes de empezar a medir, luego mide y
	 * calcula la cantidad de clocks 
	 **/
	empezar_medicion(ts);
	result = bn_mod_n( b, n, n );
	terminar_medicion(ts);

	/**
	 * imprime el resultado
	 **/
	cout << x1 << "^" << x2 << " mod " << x2 << " = " << result;
	cout << "\t\t[" << ts << "]" << endl;
	
	/**
	 * devuelve el string a almacenar en el archivo de salida
	 **/
	ostringstream out;
	out << result;
	return out.str();
} 

int main( int argc, char** argv  )
{

	procesarArchivo( , "Tp1Ej1.out", procesar_input );

	return 0;
}

