%include "macros.asm"

%macro procesarFragmentoX 5
	pmullw %1,xmm5			; xmm0 = [8px]*[línea de la matríz]
					; [a,B,c,d,E,f,o,o]
					; b<-a+b+c,E<-d+e+f (b y e son ceros!)

	pshufd xmm3,%1, %3		;? [E,F,C,D,Z,Z,Z,Z]
	pshuflw xmm3,xmm3,%4

	paddw %1,xmm3			; xmm0 == [a+c,-,-,d+f,-,-,-,-]
	pslldq %1,2			; xmm0 == [-,a+c,-,-,d+f,-,-,-]
	movdqu xmm4,[%5]
	andps %1,xmm4			; xmm0 == [0,a+c,0,0,d+f,0,0,0]
	paddw %2,%1			; xmm7 += xmm0

%endmacro

%macro procesarLineaX 3
	movdqu xmm2, %1			; duplico xmm0 en xmm2 para futuro uso
	movdqu xmm5, [%3]		; xmm5 = línea de la matríz a aplicar

	procesarFragmentoX %1, %2, 00000110b, 01000010b, mask_borrar_px1

	movdqu %1, xmm2
	pslldq xmm5,2			; xmm5 == [0,-1,0,1,-1,0,1,-1]
	procesarFragmentoX %1, %2, 00001100b, 00000010b, mask_borrar_px2

	movdqu %1, xmm2
	pslldq xmm5,2			; xmm5 == [0,0,-1,0,1,-1,0,1]
	procesarFragmentoX %1, %2, 00111000b, 11100100b, mask_borrar_px3
%endmacro

%macro aplicarLineaX 2			; parámetros: puntero a src, línea de la matriz | resultado en xmm7
;	pushad				; guardo todos los registros de propósito general
	mov esi, %1			; esi = puntero a src

	movdqu xmm0,[esi]		; xmm0 = 16 pixels de src
	movdqu xmm1, xmm0

	pxor xmm5, xmm5			; xmm5 = 0
	punpcklbw xmm0,xmm5		; xmm0 = primeros ocho px convertidos a word
	punpckhbw xmm1,xmm5		; xmm1 = segundos ocho px convertidos a word

; proceso la parte baja
	procesarLineaX xmm0, xmm7, %2

; proceso los pixeles del medio
	movdqu xmm0,xmm2
	movdqu xmm4,[mask_2words_masSig]
	andps xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,g,h]
	movdqu xmm3,xmm1		;xmm3 == parteALTA
	movdqu xmm4,[mask_2words_menSig]
	andps xmm3,xmm4			;xmm3 == [aH,bH,0,0,0,0,0,0]
	paddw xmm0,xmm3			;xmm0 == [aH,bH,0,0,0,0,g,h]
	pshufd xmm0,xmm0,00111100b	;xmm0 == [aH,bH,g,h,g,h,aH,bH]
	pshuflw xmm3,xmm0,00001100b	;xmm3 == [++,-h,+,+,+,+,-g,++]
	pshufhw xmm3,xmm3,00000000b
	movdqu xmm4,[neg_1]
	pmullw xmm3,xmm4
	
	paddw xmm0,xmm3			;xmm0 == [- ,AH,-,-,--,-,HH, -]
	pshuflw xmm0,xmm0,00000001b
	pshufhw xmm0,xmm0,10000000b
	movdqu xmm4,[mask_primYult]
	andps xmm0,xmm4			;xmm0 == [AH,-,-,-,-,-,-,-,HH]
	movdqu xmm6,xmm0
	movdqu xmm4,[mask_2words_menSig]
	andps xmm6,xmm4			;xmm6 == [aH,0,0,0,0,0,0,0]
	movdqu xmm4,[mask_2words_masSig]
	andps xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,h]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,h]

; proceso la parte alta
	procesarLineaX xmm1, xmm6, %2
	
	;movdqu xmm7, [test_mask]
;	popad				; vuelvo todos los registros a como estaban antes de llamar a la macro
%endmacro

section .data
	acum_lineas : DQ 0,0,0,0
	acum_filas : DD 0
	sobel_X_linea1: DW -1,0,1,-1,0,1,-1,0
	sobel_X_linea2: DW -2,0,2,-2,0,2,-2,0
	mask_borrar_px1: DQ 0x00000000ffff0000, 0x000000000000ffff
	mask_borrar_px2: DQ 0x0000ffff00000000, 0x00000000ffff0000
	mask_borrar_px3: DQ 0xffff000000000000, 0x0000ffff00000000
	neg_1: DQ -1,-1
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
	mask_primYult: DQ 0x000000000000ffff,0xffff000000000000

section .text
	global asmSobel

asmSobel:
	convencion_C		
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	
	xor ecx, ecx			; ecx = 0 [contador de columnas]
	xor ebx, ebx			; ebx = 0 [contador de filas]
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	mov edi, ptr_dst		; edi = puntero al inicio de línea dst

ciclo_xorder:
	pxor xmm7,xmm7			; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6			; xmm6 = 0 [acumulador parte alta]

;	lea edx, [esi+ecx]
	aplicarLineaX esi, sobel_X_linea1	; calcula en xmm6:xmm7 la primer línea
;	add edx, width
;	aplicarLineaX edx, sobel_X_linea2
;	add edx, width
;	aplicarLineaX edx, sobel_X_linea1

	;packuswb xmm7, xmm6		; convierte de word a byte saturado ambos resultados y los deja en xmm7
	mov edi,ptr_dst
	movdqu [edi], xmm7		; copia el resultado a memoria
	movdqu [edi+16],xmm6
	convencion_C_fin
	ret


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

