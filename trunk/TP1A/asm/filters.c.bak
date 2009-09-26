#include "filters.h"

const int OPERADOR_SOBEL_X[3][3] = {
	-1, 0, 1,
	-2, 0, 2,
	-1, 0, 1,
};

const int OPERADOR_SOBEL_Y[3][3] = {
	-1, -2, -1,
	 0,  0,  0,
	 1,  2,  1,
};

/**
 * cSobel - procesa la imagen con el operador de Sobel
 *		src		array de pixels ordenados (grayscale)
 *		dst		array de pixels final
 *		ancho	ancho (en pixels) de la imagen
 *		alto	alto (en pixels) de la imagen
 *		xorder	orden de la derivada x
 *		yorder	orden de la derivada y
 */

void cSobel( const unsigned char* src, unsigned char *dst, int ancho, int alto, int xorder, int yorder ) {

	int x, y, i,j,k=0;

	int fill = 4-ancho%4;
	if( fill == 4 ) fill=0;

	int row = ancho+fill;

	for( y = 1 ; y < alto-1 ; y++ )
		for( x = 1 ; x < ancho-1 ; x++ ) {
			k=0;
			for( i = 0 ; i < 3 ; i++ )
				for( j = 0 ; j < 3 ; j++ )
					k += xorder * OPERADOR_SOBEL_X[i][j]*src[row*(y+i-1)+x+j-1];
			for( i = 0 ; i < 3 ; i++ )
				for( j = 0 ; j < 3 ; j++ )
					k += yorder * OPERADOR_SOBEL_Y[i][j]*src[row*(y+i-1)+x+j-1];
			if( k > 255 )
				dst[row*y+x] = k;
			else if( k < 0 )
				dst[row*y+x] = 0;
			else
				dst[row*y+x] =  k;
		}

}
