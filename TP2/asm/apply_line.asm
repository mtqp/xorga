%include "macros.asm"

section .data
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
	global apply_lineX		;debo pasar *src y *dst
					;procesar 14 elementos!
apply_lineX:
	;convencion_C
	pushad				;conviene para poder usar todos los reg
	
	%define ptr_src [ebp+8]
;	%define ptr_dst [ebp+12]
	%define select_line2 [ebp+12]
	
	mov esi, ptr_src		; puntero al inicio de línea src
;	mov edi, ptr_dst		; puntero al inicio de línea dst

	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,xmm0

	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas
	punpckhbw xmm1,xmm7		;tengo las 1ras ocho words altas
				
	;TRABAJO PRIMERO CON PARTE BAJA, PARTE ALTA ANALOGA
	movdqu xmm2,xmm0		;salvo x un momento la matriz
	cmp dword select_line2,0x01	;cual de los 2 tipos de linea proceso?
	je line2
	movdqu xmm5,[sobel_X_linea1]
	jmp continuar
line2:
	movdqu xmm5,[sobel_X_linea2]
continuar:
	pmullw xmm0,xmm5		;[a,B,c,d,E,f,o,o]
					;b<-a+b+c,E<-d+e+f (b y e son ceros!)
	pshufd xmm3,xmm0,00000110b	;? [E,F,C,D,Z,Z,Z,Z]
	pshuflw xmm3,xmm3,01000010b
				;xmm3 == [C,fruta,fruta,F,fruta,fruta,gdaf,graf]
				;xmm0 == [A,frefr,frefr,D,frefr,frefr,fref,fref]
	paddw xmm0,xmm3		;xmm0 == [B,fruta,fruit,E,fruit,fruit,fruit,fru]
	pslldq xmm0,2		;xmm0 == [frut,B,fruit,fruto,E,fruti,frtu,friut]
	movdqu xmm4,[mask_borrar_px1]
	andps xmm0,xmm4		;xmm0 == [0,B,0,0,E,0,0,0]
	paddw xmm7,xmm0			;xmm7 == xmm0
					
	movdqu xmm0,xmm2
	pslldq xmm5,2			;xmm5 == [0,-1,0,1,-1,0,1,-1]
	pmullw xmm0,xmm5		;[o,b,C,d,e,F,g,o]
					;C<-b+c+d,F<-e+f+g	(c y f == 0)
	pshuflw xmm3,xmm0,00001100b	;xmm3 == [-,d,-,-,e,f,g,-]
	pshufhw xmm3,xmm3,00000010b	;xmm3 == [-,d,-,-,g,-,-,-]
					;xmm0 == [-,b,-,-,e,-,-,-]
	paddw xmm0,xmm3			;xmm0 == [-,C,-,-,F,-,-,-]
	pslldq xmm0,2			;xmm0 == [-,-,C,-,-,F,-,-]
	movdqu xmm4,[mask_borrar_px2]
	andps xmm0,xmm4			;xmm0 == [0,0,C,0,0,F,0,0]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,0,e,f,0,0]
					
	movdqu xmm0,xmm2
	pslldq xmm5,2			;xmm5 ==  [0,0,-1,0,1,-1,0,1]
	pmullw xmm0,xmm5		;xmm0 == [-,-,c,D,e,f,G,h]
	
	pshufd xmm3,xmm0,00111000b	;xmm3 == [-,-,e,f,g,h,-,-]
	paddw xmm0,xmm3			;xmm0 == [-,-,D,-,-,G,-,-]
	pslldq xmm0,2			;xmm0 == [-,-,-,D,-,-,G,-]
	movdqu xmm4,[mask_borrar_px3]
	andps xmm0,xmm4
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,0]
					
				;falta ver mas signific de la parte baja
				;y el menos sig de la parte alta
bordes_xmm:			;en xmm1 estan las ocho words altas
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
	

parteAlta:
	movdqu xmm2,xmm1		;salvo x un momento la matriz
	cmp dword select_line2,0x01
	je line2H
	movdqu xmm5,[sobel_X_linea1]
	jmp continuarH
line2H:
	movdqu xmm5,[sobel_X_linea2]
continuarH:
	pmullw xmm1,xmm5		;[a,B,c,d,E,f,o,o]
					;b<-a+b+c,E<-d+e+f (b y e son ceros!)
	pshufd xmm3,xmm1,00000110b	;? [E,F,C,D,Z,Z,Z,Z]
	pshuflw xmm3,xmm3,01000010b
				;xmm3 == [C,fruta,fruta,F,fruta,fruta,gdaf,graf]
				;xmm0 == [A,frefr,frefr,D,frefr,frefr,fref,fref]
	paddw xmm1,xmm3		;xmm0 == [B,fruta,fruit,E,fruit,fruit,fruit,fru]
	pslldq xmm1,2		;xmm0 == [frut,B,fruit,fruto,E,fruti,frtu,friut]
	movdqu xmm4,[mask_borrar_px1]
	andps xmm1,xmm4		;xmm0 == [0,B,0,0,E,0,0,0]
	paddw xmm6,xmm1			;xmm7 == xmm0
					
	movdqu xmm1,xmm2
	pslldq xmm5,2			;xmm5 == [0,-1,0,1,-1,0,1,-1]
	pmullw xmm1,xmm5		;[o,b,C,d,e,F,g,o]
					;C<-b+c+d,F<-e+f+g	(c y f == 0)
	pshuflw xmm3,xmm1,00001100b	;xmm3 == [-,d,-,-,e,f,g,-]
	pshufhw xmm3,xmm3,00000010b	;xmm3 == [-,d,-,-,g,-,-,-]
					;xmm0 == [-,b,-,-,e,-,-,-]
	paddw xmm1,xmm3			;xmm0 == [-,C,-,-,F,-,-,-]
	pslldq xmm1,2			;xmm0 == [-,-,C,-,-,F,-,-]
	movdqu xmm4,[mask_borrar_px2]
	andps xmm1,xmm4			;xmm0 == [0,0,C,0,0,F,0,0]
	paddw xmm6,xmm1			;xmm7 == [0,b,c,0,e,f,0,0]
					
	movdqu xmm1,xmm2
	pslldq xmm5,2			;xmm5 ==  [0,0,-1,0,1,-1,0,1]
	pmullw xmm1,xmm5		;xmm0 == [-,-,c,D,e,f,G,h]
	
	pshufd xmm3,xmm1,00111000b	;xmm3 == [-,-,e,f,g,h,-,-]
	paddw xmm1,xmm3			;xmm0 == [-,-,D,-,-,G,-,-]
	pslldq xmm1,2			;xmm0 == [-,-,-,D,-,-,G,-]
	movdqu xmm4,[mask_borrar_px3]
	andps xmm1,xmm4
	paddw xmm6,xmm1			;xmm7 == [0,b,c,d,e,f,g,0]
					

fin_apply_line:

;	packuswb xmm7,xmm6
	;movdqu [edi],xmm7
	;convencion_C_fin
	popad
	ret

