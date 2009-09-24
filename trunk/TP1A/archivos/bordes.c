#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>


int main( int argc, char** argv )
{

   IplImage * src = 0;
   IplImage * dst = 0;
   IplImage * dst_ini = 0;

   char* filename = argc == 3 ? argv[2] : (char*)"lena.bmp";
   
	switch((char)argv[1][2]){
		
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
            break;
	}

   // Cargo la imagen
   if( (src = cvLoadImage (filename, CV_LOAD_IMAGE_GRAYSCALE)) == 0 ){
   	   printf("Error cargando la imagen fuente...\n");
	   return -1;
	}
	
   // Creo una IplImage para cada salida esperada
   if( (dst = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 ){
   	   printf("Error creando la imagen destino...\n");
	   return -1;
	}

   // Creo una IplImage para cada salida esperada
   if( (dst_ini = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 ){
   	   printf("Error creando la imagen dst_ini...\n");
	   return -1;
	}

   // Aplico el filtro (Sobel con derivada x en este caso) y salvo imagen 
   cvSobel(src, dst, 1,0,3); 	// Esta parte es la que tienen que programar los alumnos en ASM	y comparar
   cvSaveImage("derivada x.BMP", dst);

   // Aplico el filtro (Sobel con derivada y en esta caso) y salvo imagen 
   cvSobel(src, dst, 0,1,3);    // Esta parte es la que tienen que programar los alumnos en ASM	y comparar
   cvSaveImage("derivada y.BMP", dst);


   return 0;

}


