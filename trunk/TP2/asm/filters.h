#ifndef __FILTERS_H__
#define __FILTERS_H__

/** toCharS - convierte x a char mediante saturación **/
#define toCharS(x) x > 255? 255 : ( x < 0 ? 0 : x )

/** lineSize - obtiene el tamaño de la línea incluyendo el alineado  **/
#define lineSize(width,align) width-width%align+align*(width%align>0)

void cSobel( const unsigned char* src, unsigned char *dst, int ancho, int alto, int xorder, int yorder );
/*extern asmSobel  ( const unsigned char* src, unsigned char *dst, int ancho, int alto, int xorder, int yorder );
extern asmRoberts( const unsigned char* src, unsigned char *dst, int ancho, int alto, int xorder, int yorder );
extern asmPrewitt( const unsigned char* src, unsigned char *dst, int ancho, int alto, int xorder, int yorder );
//int apply_mask( const unsigned char* src, unsigned int line, const char *mask, unsigned int mask_sz );*/

extern int apply_mask( const unsigned char* src, unsigned int line, const char *mask, unsigned int mask_sz );

typedef void(FILTERFN)( const unsigned char* src, unsigned char* dst, int width, int height, int xorder, int yorder );
extern FILTERFN asmSobel;
extern FILTERFN asmRoberts;
extern FILTERFN asmPrewitt;

#endif // __FILTERS_H__
