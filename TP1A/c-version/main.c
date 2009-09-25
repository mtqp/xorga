#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "filters.h"

int main( int argc, char** argv )
{
	char* filename = argc == 3 ? argv[2] : (char*)"lena.bmp";
	char opcion='0';

	IplImage * src = 0;
	IplImage * dst = 0;
	//IplImage * dst_ini = 0; // Para qué se debería usar??

	//Me aseguro del comando...
	if( argc>1 && argv[1][0] == '-' && argv[1][1] == 'r' && argv[1][2]>'0' 
		&& argv[1][2]<'6' && argv[1][3]=='\0')
		opcion = argv[1][2];

	switch(opcion){
		case '1':
			printf( "Usando el operador de Roberts para realzar bordes\n");
			break;
		case '2':
			printf( "Usando el operador de Prewitt para realzar bordes\n");
			break;
		case '3':
			printf( "Usando el operador de Sobel para realzar bordes\nDerivando en X\n");
			break;
		case '4':
			printf( "Usando el operador de Sobel para realzar bordes\nDerivando en Y\n"); 
			break;
		case '5':
			printf( "Usando el operador de Sobel para realzar bordes\nDerivando en X e Y\n"); 
			break;
		default:
			printf( "\nRealzador de Bordes TP1 OrgaII Grupo XoOoOoOoR (cambiar..)\n\n");
			printf( "Uso:\n     ./bordes operador [filename] \n\n");
			printf( "Operadores posibles:\n");
			printf( "    -r1: Operador de Roberts \n");
			printf( "    -r2: Operador de Prewitt\n");
			printf( "    -r3: Operador de Sobel derivando por X\n");
			printf( "    -r4: Operador de Sobel derivando por Y\n");
			printf( "    -r5: Operador de Sobel derivando por X e Y\n\n");
			printf( "Si no se especifica un archivo de entrada, se usara 'lena.bmp'\n\n");
			return 0;
	}

	// Cargo la imagen
	if( (src = cvLoadImage (filename, CV_LOAD_IMAGE_GRAYSCALE)) == 0 ) {
		printf("ERROR AL CARGAR LA IMAGEN %s\n", filename);
		return -1;
	}

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 )
		return -1;

	// Creo una IplImage para cada salida esperada
	//if( (dst_ini = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) ==0 )
	//	return -1;

	switch(opcion) {
		case '1':
			// Aplico el filtro (Roberts con derivada x en este caso) y salvo imagen
			cRoberts(src->imageData, dst->imageData,src->width,src->height, 1,1);		// Esta parte es la que tienen que programar los alumnos en ASM	y comparar
			cvSaveImage("roberts.bmp", dst);
			break;
		case '2':
			// Aplico el filtro (Roberts con derivada x en este caso) y salvo imagen
			cPrewitt(src->imageData, dst->imageData,src->width,src->height, 1,1);		// Esta parte es la que tienen que programar los alumnos en ASM	y comparar
			cvSaveImage("prewitt.bmp", dst);
			break;
		case '3':
			// Aplico el filtro (Sobel con derivada x en este caso) y salvo imagen
			cSobel(src->imageData, dst->imageData,src->width,src->height, 1,0); 	// Esta parte es la que tienen que programar los alumnos en ASM	y comparar
			cvSaveImage("derivada-x.bmp", dst);
			break;
		case '4':
			// Aplico el filtro (Sobel con derivada y en este caso) y salvo imagen
			cSobel(src->imageData, dst->imageData,src->width,src->height, 0,1);	// Esta parte es la que tienen que programar los alumnos en ASM y comparar
			cvSaveImage("derivada-y.bmp", dst);
			break;
		case '5':
			// Aplico el filtro (Sobel con derivada y en esta caso) y salvo imagen
			cSobel(src->imageData, dst->imageData,src->width,src->height, 1,1);	// Esta parte es la que tienen que programar los alumnos en ASM y comparar
			cvSaveImage("sobel.bmp", dst);
			/*cvSobel(src,dst,1,0,3);
			cvSaveImage("sobel-orig-x.bmp", dst);
			cvSobel(src,dst,0,1,3);
			cvSaveImage("sobel-orig-y.bmp", dst);*/
	}
	//printf("OK\n");
	cvReleaseImage( &dst );
	cvReleaseImage( &src );
	return 0;
}
