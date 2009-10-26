;///aridad funcion      void apply_lineRob(char * ptr_src,char* ptr_dst,int witdh_line)

%include "macros.asm"

;matriz roberts
;1	0
;0	-1

section .data
	matriz_10: DQ 0x0000ffff0000ffff, 0x0000ffff0000ffff
	neg_matriz_01: DQ 0xffff0000ffff0000,0xffff0000ffff0000
	dejar_masSig: DQ 0, 0xffff000000000000

;	mask_borrar_px1: DQ 0x00000000ffff0000, 0x000000000000ffff
;	mask_borrar_px2: DQ 0x0000ffff00000000, 0x00000000ffff0000
;	mask_borrar_px3: DQ 0xffff000000000000, 0x0000ffff00000000
;	neg_1: DQ -1,-1
;	mask_2words_masSig: DQ 0, 0xffffffff00000000
;	mask_2words_menSig: DQ 0x00000000ffffffff, 0
;	mask_primYult: DQ 0x000000000000ffff,0xffff000000000000
section .text
	global apply_lineRob	;debo pasar *src y *dst
							;procesar 14 elementos!
	

apply_lineRob:
	convencion_C
		
	%define ptr_src [ebp+8]
	%define ptr_dst [ebp+12]
	%define width [ebp+16]
	
	mov esi, ptr_src		; puntero al inicio de línea src
	mov edi, ptr_dst		; puntero al inicio de línea dst
	mov ebx, width
	
	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

	
parteBaja:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[neg_matriz_01]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas de 1 0
	punpcklbw xmm1,xmm7		;tengo las 1ras ocho words bajas de 0 -1
				
	;TRABAJO PRIMERO CON PARTE BAJA, PARTE ALTA ANALOGA
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1


	pand xmm0,xmm4			;xmm0 == [a,0,c,0,e,0,g,0,]
	pand xmm1,xmm5 		;xmm1 == [0,-b,0,-d,0,-f,0,-h]
	psrldq xmm1,2			;xmm1 == [-b,0,-d,0,-f,0,-h,0]
	paddw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm7,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,0]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,0]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,0]
	paddw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,0]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,0]
	
	
px_masSig_parteBaja:
	movdqu xmm0,[esi]
	movdqu xmm1,[esi+ebx]
	pand xmm0,[dejar_masSig];xmm0 == [0,0,0,0,0,0,0,h]
	pslldq xmm1,14			;xmm1 == [0,0,0,0,0,0,0,aH]
	psubw xmm0,xmm1			;xmm0 == [0,0,0,0,0,0,0,H]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,H]
	
parteAlta:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[neg_matriz_01]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpckhbw xmm0,xmm6		;tengo las primeras ocho words altas de 1 0
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas de 0 -1
				
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1


	pand xmm0,xmm4			;xmm0 == [a,0,c,0,e,0,g,0,]
	pand xmm1,xmm5 		;xmm1 == [0,-b,0,-d,0,-f,0,-h]
	psrldq xmm1,2			;xmm1 == [-b,0,-d,0,-f,0,-h,0]
	paddw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm6,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,0]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,0]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,0]
	paddw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,0]
	paddw xmm6,xmm0			;xmm6 == [A,B,C,D,E,F,G,0]
	
save_mem:
	packuswb xmm7, xmm6		;convierte de word a byte saturado ambos resultados y los deja en xmm7	
	movdqu [edi],xmm7
	
	convencion_C_fin
	ret

