#include "filters.h"

const char OPERADOR_SOBEL_X[9] = {
	-1, 0, 1,
	-2, 0, 2,
	-1, 0, 1
};

const char OPERADOR_SOBEL_Y[9] = {
	-1, -2, -1,
	 0,  0,  0,
	 1,  2,  1
};

const char OPERADOR_ROBERTS_X[4] = {
	1,  0,
	0, -1
};

const char OPERADOR_ROBERTS_Y[4] = {
	 0, 1,
	-1, 0
};

const char OPERADOR_PREWITT_X[9] = {
	-1, 0, 1,
	-1, 0, 1,
	-1, 0, 1
};

const char OPERADOR_PREWITT_Y[9] = {
	-1, -1, -1,
	 0,  0,  0,
	 1,  1,  1
};

/**
 * cX - procesa la imagen con el operador X
 *		src	array de pixels ordenados (grayscale)
 *		dst	array de pixels final
 *		width	ancho (en pixels) de la imagen
 *		height	alto (en pixels) de la imagen
 *		xorder	orden de la derivada x
 *		yorder	orden de la derivada y
 */
void cSobel( const unsigned char* src, unsigned char *dst, int width, int height, int xorder, int yorder ) {
	int x, y, k;
	int line = lineSize(width,4);

	for( y = 1 ; y < height-1 ; y++ )
		for( x = 1 ; x < width-1 ; x++ ) {
			k=0;
			if( xorder != 0 )
				k =toCharS(apply_mask( &src[line*(y-1)+x-1], line, OPERADOR_SOBEL_X, 3 ));
			if( yorder != 0 )
				k+=toCharS(apply_mask( &src[line*(y-1)+x-1], line, OPERADOR_SOBEL_Y, 3 ));
			dst[line*y+x]=toCharS(k);
		}
}

void cRoberts( const unsigned char* src, unsigned char *dst, int width, int height, int xorder, int yorder ) {
	int x, y, k;
	int line = lineSize(width,4);

	for( y = 0 ; y < height-1 ; y++ )
		for( x = 0 ; x < width-1 ; x++ ) {
			k=0;
			if( xorder != 0 )
				k =toCharS(abs(apply_mask( &src[line*y+x], line, OPERADOR_ROBERTS_X, 2 )));
			if( yorder != 0 )
				k+=toCharS(abs(apply_mask( &src[line*y+x], line, OPERADOR_ROBERTS_Y, 2 )));
			dst[line*y+x]=toCharS(k);
		}
}

void cPrewitt( const unsigned char* src, unsigned char *dst, int width, int height, int xorder, int yorder ) {
	int x, y, k;
	int line = lineSize(width,4);

	for( y = 1 ; y < height-1 ; y++ )
		for( x = 1 ; x < width-1 ; x++ ) {
			k=0;
			if( xorder != 0 )
				k =toCharS(abs(apply_mask( &src[line*(y-1)+x-1], line, OPERADOR_PREWITT_X, 3 )));
			if( yorder != 0 )
				k+=toCharS(abs(apply_mask( &src[line*(y-1)+x-1], line, OPERADOR_PREWITT_Y, 3 )));
			dst[line*y+x]=toCharS(k);
		}
}

/**
 * apply_mask - aplica la máscara y devuelve el resultado
 *		src		array de pixels ordenados (grayscale)
 *		line		tamaño de línea del array (incluyendo el fill)
 *		mask		máscara a aplicar
 *		mask_sz		tamaño del lado de la máscara
 */
int apply_mask( const unsigned char* src, unsigned int line, const char* mask, unsigned int mask_sz ) {
	int i,j,k=0;
	for( i = 0 ; i < mask_sz ; i++ )
		for( j = 0 ; j < mask_sz ; j++ )
			k += mask[mask_sz*i+j] * src[line*i+j];
	return k;
}

