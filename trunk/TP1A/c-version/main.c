#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "filters.h"

int main( int argc, char** argv )
{

   IplImage * src = 0;
   IplImage * dst = 0;
   IplImage * dst_ini = 0; // Para qué se debería usar??

   char* filename = argc == 2 ? argv[1] : (char*)"lena.bmp";

   // Cargo la imagen
   if( (src = cvLoadImage (filename, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
	   return -1;

   // Creo una IplImage para cada salida esperada
   if( (dst = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 )
	   return -1;

   // Creo una IplImage para cada salida esperada
   if( (dst_ini = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 )
	   return -1;

   // Aplico el filtro (Sobel con derivada x en este caso) y salvo imagen 
   cSobel(src->imageData, dst->imageData,512,512, 1,0); 	// Esta parte es la que tienen que programar los alumnos en ASM	y comparar
   cvSaveImage("derivada x.BMP", dst);

   // Aplico el filtro (Sobel con derivada y en esta caso) y salvo imagen 
   cSobel(src->imageData, dst->imageData,512,512, 0,1);	// Esta parte es la que tienen que programar los alumnos en ASM y comparar
   cvSaveImage("derivada y.BMP", dst);

   // Aplico el filtro (Sobel con derivada y en esta caso) y salvo imagen 
   cSobel(src->imageData, dst->imageData,512,512, 1,1);	// Esta parte es la que tienen que programar los alumnos en ASM y comparar
   cvSaveImage("full.BMP", dst);

   return 0;

}


