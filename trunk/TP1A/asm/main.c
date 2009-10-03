#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filters.h"

void apply_filter( char filter, IplImage *src, IplImage *dst ) {
	switch(filter) {
		case '1':
			// Aplico el filtro (Roberts con derivada x en este caso)
			asmRoberts(src->imageData, dst->imageData,src->width,src->height, 1,1);
			break;
		case '2':
			// Aplico el filtro (Roberts con derivada x en este caso)
			asmPrewitt(src->imageData, dst->imageData,src->width,src->height, 1,1);
			break;
		case '3':
			// Aplico el filtro (Sobel con derivada x en este caso)
			asmSobel(src->imageData, dst->imageData,src->width,src->height, 1,0);
			break;
		case '4':
			// Aplico el filtro (Sobel con derivada y en este caso)
			asmSobel(src->imageData, dst->imageData,src->width,src->height, 0,1);
			break;
		case '5':
			// Aplico el filtro (Sobel con derivada x e y en esta caso)
			asmSobel(src->imageData, dst->imageData,src->width,src->height, 1,1);
			
			break;			
	}
}

void save_image( char *filename, IplImage *dst, char filter ) {
	char *filterName, *finalName;
	char len = strlen(filename);
	// Establece el nombre del filtro
	if( filter == '0' ) filterName = (char*)"-grayscale";
	else if( filter == '1' ) filterName = (char*)"-roberts";
	else if( filter == '2' ) filterName = (char*)"-prewitt";
	else if( filter == '3' ) filterName = (char*)"-sobel-x";
	else if( filter == '4' ) filterName = (char*)"-sobel-y";
	else if( filter == '5' ) filterName = (char*)"-sobel";
	// Compone el nombre del archivo final con la forma nombre_original-filtro.extension_original
	finalName = (char*)malloc( len + strlen(filterName)+1 );
	strncpy( finalName, filename, len-4 );
	strcpy( (char*)(finalName+len-4), filterName );
	strncpy( (char*)(finalName+len-4+strlen(filterName)), (char*)(filename+len-4),4 );
	finalName[len+strlen(filterName)]='\0';
	// Guarda la imagen
	cvSaveImage( finalName, dst );
	printf( "Guardado %s\n", finalName );
	free( finalName );
}

void show_window( char* wndTitle, char* filename, IplImage *src, IplImage *dst, char filter ) {
	char key;
	// Crea la ventana y muestra la imagen correspondiente al filtro seleccionado
	cvNamedWindow( wndTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage( wndTitle, filter=='0'? src : dst );
	// Mientras que la ventana siga existiendo ...
	while(cvGetWindowHandle(wndTitle)){
		// Obtiene la tecla presionada
		key = cvWaitKey(100);
		// Llama a la función correspondiente a la tecla presionada
		if( key == '0' ) cvShowImage( wndTitle, src);
		else if( key == 's' ) save_image( filename, filter=='0'? src : dst, filter );
		else if( key == 'q' || key=='\e' ) return;
		else if( key > '0' && key <= '5' ) {
			apply_filter( key, src, dst );
			cvShowImage( wndTitle, dst );
			filter = key;
		}
	}
}

void showMsg( char filter, char window_mode ) {
	printf( "\nRealzador de Bordes TP1 OrgaII Grupo XOR \n\n");
	switch(filter){
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
		case '0':
			break;
		default:
			printf( "Uso:\n     ./bordes {-r# | -g} [filename] \n\n");
			printf( "Opciones:\n");
			printf( "    -r#\t\tAplica el operador # \n");
			printf( "    -g\t\tModo Gráfico \n\n");
			printf( "Operadores posibles:\n");
			printf( "    1: Operador de Roberts \n");
			printf( "    2: Operador de Prewitt\n");
			printf( "    3: Operador de Sobel derivando por X\n");
			printf( "    4: Operador de Sobel derivando por Y\n");
			printf( "    5: Operador de Sobel derivando por X e Y\n\n");
			printf( "Si no se especifica un archivo de entrada, se usará 'lena.bmp'\n\n");
			return;
	}

	if( window_mode ) {
		printf( "Uso:\n     Presionar la tecla correspondiente para aplicar el filtro \n\n");
		printf( "Opciones:\n");
		printf( "    1: Operador de Roberts \n");
		printf( "    2: Operador de Prewitt\n");
		printf( "    3: Operador de Sobel derivando por X\n");
		printf( "    4: Operador de Sobel derivando por Y\n");
		printf( "    5: Operador de Sobel derivando por X e Y\n");
		printf( "    0: Escala de grises\n");
		printf( "    s: Guardar\n");
		printf( "    q: Salir\n\n");
	}

}

int main( int argc, char** argv )
{
	char* filename = (char*)"lena.bmp";
	char filter=0, window_mode = 0;
	int i;

	IplImage * src = 0;
	IplImage * dst = 0;
	
	// Analizo las opciones pasadas por línea de comandos
	for( i = 1 ; i < argc ; i++ ) {
		if( argv[i][0]=='-' )
		switch( argv[i][1] ) {
			case 'g': 
				// Modo gráfico, por defecto usa escala de grises
				window_mode = 1;
				if( !filter ) filter = '0';
				break;
			case 'r':
				// Filtro = 0 si es inválido, de '1' a '5' si es válido
				filter = (argv[i][2]>'0' && argv[i][2]<'6' && argv[i][3]=='\0' ) * argv[i][2];
		}
		else
			filename = argv[i];
	}

	// Muestra el mensaje correspondiente a la opción solicitada
 	showMsg( filter, window_mode );

	// Si no se especificó un filtro sale del programa
	if( filter == 0 )
		return;

	// Carga la imagen
	if( (src = cvLoadImage (filename, CV_LOAD_IMAGE_GRAYSCALE)) == 0 ) {
		printf("ERROR AL CARGAR LA IMAGEN %s\n", filename);
		return -1;
	}

	// Crea una IplImage para la salida
	if( (dst = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 )
		return -1;

	// Aplica el filtro correspondiente a las opciones
	apply_filter( filter, src, dst );

	// Si está en modo ventana, abre la ventana. Sino guarda la imagen.
	if(window_mode)
		show_window( "Grupo XOR - Detección de bordes", filename, src, dst, filter );
	else
		save_image( filename, dst, filter );
	
	// Libera la memoria
	cvReleaseImage( &dst );
	cvReleaseImage( &src );
	cvDestroyAllWindows();
	return 0;
}
