#ifndef __FILTERS_H__
#define __FILTERS_H__

/** toCharS - convierte x a char mediante saturación **/
#define toCharS(x) x > 255? 255 : ( x < 0 ? 0 : x )

/** lineSize - obtiene el tamaño de la línea incluyendo el alineado  **/
#define lineSize(width,align) width-width%align+align*(width%align>0)

/** abs - calcula el valor absoluto de una expresión **/
#define abs(x) x>0?x:-x


typedef void(FILTERFN)( const unsigned char* src, unsigned char* dst, int width, int height, int xorder, int yorder );
FILTERFN cSobel;
FILTERFN cRoberts;
FILTERFN cPrewitt;

int apply_mask( const unsigned char* src, unsigned int line, const char *mask, unsigned int mask_sz );

#endif // __FILTERS_H__
