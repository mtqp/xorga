%macro robX 0
;///////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////
apply_RobX:
	pushad	
	%define ptr_src [ebp+8]
	%define width [ebp+16]
	
	mov edi, acum_linex		; puntero al inicio de línea dst
	mov ebx, width
		
	lea esi,[esi+ecx]
	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

parteBaja_robX:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[matriz_01]

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
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm7,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,-]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,-]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm7,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]
	
px_masSig_parteBaja_robX:
	pxor xmm3,xmm3
	movdqu xmm0,[esi]
	movdqu xmm1,[esi+ebx]
	movdqu xmm4,[dejar_masSig]
	punpcklbw xmm0,xmm3		;necesito el mas sig de la linea menos sig		
	punpckhbw xmm1,xmm3		;contra el menos sig de la linea mas sig
	pand xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,h]
	pslldq xmm1,14			;xmm1 == [0,0,0,0,0,0,0,aH]
	psubw xmm0,xmm1			;xmm0 == [0,0,0,0,0,0,0,H]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,H]
	
parteAlta_robX:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[matriz_01]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpckhbw xmm0,xmm6		;tengo las primeras ocho words altas de 1 0
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas de 0 -1
				
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [a,0,c,0,e,0,g,0,]
	pand xmm1,xmm5	 		;xmm1 == [0,-b,0,-d,0,-f,0,-h]
	psrldq xmm1,2			;xmm1 == [-b,0,-d,0,-f,0,-h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm6,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,0]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,0]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,0]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,0]
	paddw xmm6,xmm0			;xmm6 == [A,B,C,D,E,F,G,0]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm6,xmm4			;xmm6 == [A,B,C,D,E,F,G,0]
	
save_mem_robX:
	packuswb xmm7, xmm6		;convierte de word a byte saturado ambos resultados y los deja en xmm7	
	movdqu [edi],xmm7
	
	popad
;//////////////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////////////////////////	
;//////////////////////////////////////////////////////////////////////////////////////////	
%endmacro

%macro robY 0
apply_RobY:
	pushad
	%define ptr_src [ebp+8]
	%define width [ebp+16]
	
	mov edi, acum_liney		; puntero al inicio de línea dst
	mov ebx, width
	
	lea esi,[esi+ecx]
	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

parteBaja_robY:
	movdqu xmm4,[matriz_01]
	movdqu xmm5,[matriz_10]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas de 1 0
	punpcklbw xmm1,xmm7		;tengo las 1ras ocho words bajas de 0 -1
				
	;TRABAJO PRIMERO CON PARTE BAJA, PARTE ALTA ANALOGA
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,h]
	pand xmm1,xmm5	 		;xmm1 == [a,0,c,0,e,0,g,0]	;ESTE DEBE RESTARSE
	psrldq xmm0,2			;xmm0 == [b,0,d,0,f,0,h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm7,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,0,c,0,e,0,g,0]
	pand xmm1,xmm5			;xmm1 == [0,b,0,d,0,f,0,h]	;debe restarse!
	psrldq xmm0,2			;xmm0 == [0,c,0,e,0,g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm7,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]
	
px_masSig_parteBaja_robY:
	pxor xmm3,xmm3
	movdqu xmm0,[esi+ebx]
	movdqu xmm1,[esi]
	movdqu xmm4,[dejar_masSig]
	punpcklbw xmm0,xmm3		;necesito el mas sig de la linea menos sig		
	punpckhbw xmm1,xmm3		;contra el menos sig de la linea mas sig
	pand xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,hH]
	pslldq xmm1,14			;xmm1 == [0,0,0,0,0,0,0,a]
	psubw xmm1,xmm0			;xmm1 == [0,0,0,0,0,0,0,H]
	paddw xmm7,xmm1			;xmm7 == [A,B,C,D,E,F,G,H]
	
parteAlta_robY:
	movdqu xmm4,[matriz_01]
	movdqu xmm5,[matriz_10]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpckhbw xmm0,xmm6		;tengo las primeras ocho words altas de 1 0
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas de 0 -1

	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,h]
	pand xmm1,xmm5	 		;xmm1 == [a,0,c,0,e,0,g,0]	;ESTE DEBE RESTARSE
	psrldq xmm0,2			;xmm0 == [b,0,d,0,f,0,h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm6,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,0,c,0,e,0,g,0]
	pand xmm1,xmm5			;xmm1 == [0,b,0,d,0,f,0,h]	;debe restarse!
	psrldq xmm0,2			;xmm0 == [0,c,0,e,0,g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm6,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm6,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]

save_mem_robY:
	packuswb xmm7, xmm6		;convierte de word a byte saturado ambos resultados y los deja en xmm7	
	movdqu [edi],xmm7

	popad
;////////////////////////////////////////////////////////////////////////
;////////////////////////////////////////////////////////////////////////
;////////////////////////////////////////////////////////////////////////
%endmacro

%macro ultimalinea 0
;/////tuve que poner esta macro x linea 215 y 216//////////////////////
;//////////////////////////////////////////////////////////////////////
ultlineaapply_RobX:
	pushad	
	%define ptr_src [ebp+8]
	%define width [ebp+16]
	
	mov edi, acum_linex		; puntero al inicio de línea dst
	mov ebx, width
	
	mov ecx,width			;ESTO ES NUEVO AGREGADO1!!/////////
	sub ecx,16
	
	
	lea esi,[esi+ecx]
	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

	
ultlineaparteBaja_robX:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[matriz_01]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas de 1 0
	punpcklbw xmm1,xmm7		;tengo las 1ras ocho words bajas de 0 -1
				
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [a,0,c,0,e,0,g,0,]
	pand xmm1,xmm5 		;xmm1 == [0,-b,0,-d,0,-f,0,-h]
	psrldq xmm1,2			;xmm1 == [-b,0,-d,0,-f,0,-h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm7,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,-]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,-]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm7,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]
	
ultlineapx_masSig_parteBaja_robX:
	pxor xmm3,xmm3
	movdqu xmm0,[esi]
	movdqu xmm1,[esi+ebx]
	movdqu xmm4,[dejar_masSig]
	punpcklbw xmm0,xmm3		;necesito el mas sig de la linea menos sig		
	punpckhbw xmm1,xmm3		;contra el menos sig de la linea mas sig
	pand xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,h]
	pslldq xmm1,14			;xmm1 == [0,0,0,0,0,0,0,aH]
	psubw xmm0,xmm1			;xmm0 == [0,0,0,0,0,0,0,H]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,H]
	
ultlineaparteAlta_robX:
	movdqu xmm4,[matriz_10]
	movdqu xmm5,[matriz_01]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpckhbw xmm0,xmm6		;tengo las primeras ocho words altas de 1 0
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas de 0 -1
				
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [a,0,c,0,e,0,g,0,]
	pand xmm1,xmm5	 		;xmm1 == [0,-b,0,-d,0,-f,0,-h]
	psrldq xmm1,2			;xmm1 == [-b,0,-d,0,-f,0,-h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm6,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,0]
	pand xmm1,xmm5			;xmm1 == [0,0,-c,0,-e,0,-g,0]
	psrldq xmm1,2			;xmm1 == [0,-c,0,-e,0,-g,0,0]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,0]
	paddw xmm6,xmm0			;xmm6 == [A,B,C,D,E,F,G,0]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm6,xmm4			;xmm6 == [A,B,C,D,E,F,G,0]
	
ultlineasave_mem_robX:
	packuswb xmm7, xmm6		;convierte de word a byte saturado ambos resultados y los deja en xmm7	
	movdqu [edi],xmm7
	
	popad
;//////////////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////////////////////////	
	
ultlineaapply_RobY:
	pushad
	%define ptr_src [ebp+8]
	%define width [ebp+16]
	
	mov edi, acum_liney		; puntero al inicio de línea dst
	mov ebx, width
	
	lea esi,[esi+ecx]
	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta

ultlineaparteBaja_robY:
	movdqu xmm4,[matriz_01]
	movdqu xmm5,[matriz_10]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas de 1 0
	punpcklbw xmm1,xmm7		;tengo las 1ras ocho words bajas de 0 -1
				
	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,h]
	pand xmm1,xmm5	 		;xmm1 == [a,0,c,0,e,0,g,0]	;ESTE DEBE RESTARSE
	psrldq xmm0,2			;xmm0 == [b,0,d,0,f,0,h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm7,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,0,c,0,e,0,g,0]
	pand xmm1,xmm5			;xmm1 == [0,b,0,d,0,f,0,h]	;debe restarse!
	psrldq xmm0,2			;xmm0 == [0,c,0,e,0,g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm7,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm7,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]
	
ultlineapx_masSig_parteBaja_robY:
	pxor xmm3,xmm3
	movdqu xmm0,[esi+ebx]
	movdqu xmm1,[esi]
	movdqu xmm4,[dejar_masSig]
	punpcklbw xmm0,xmm3		;necesito el mas sig de la linea menos sig		
	punpckhbw xmm1,xmm3		;contra el menos sig de la linea mas sig
	pand xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,hH]
	pslldq xmm1,14			;xmm1 == [0,0,0,0,0,0,0,a]
	psubw xmm1,xmm0			;xmm1 == [0,0,0,0,0,0,0,H]
	paddw xmm7,xmm1			;xmm7 == [A,B,C,D,E,F,G,H]
	
ultlineaparteAlta_robY:
	movdqu xmm4,[matriz_01]
	movdqu xmm5,[matriz_10]

	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,[esi+ebx]	;levanto la segunda linea

	punpckhbw xmm0,xmm6		;tengo las primeras ocho words altas de 1 0
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas de 0 -1

	movdqu xmm2,xmm0			;salvo x un momento las matrices
	movdqu xmm3,xmm1

	pand xmm0,xmm4			;xmm0 == [0,b,0,d,0,f,0,h]
	pand xmm1,xmm5	 		;xmm1 == [a,0,c,0,e,0,g,0]	;ESTE DEBE RESTARSE
	psrldq xmm0,2			;xmm0 == [b,0,d,0,f,0,h,0]
	psubw xmm0,xmm1			;xmm0 == [A,0,C,0,E,0,G,0]	-->LETRA EN MAYUS == LETRA RES
	paddw xmm6,xmm0
	
	movdqu xmm0,xmm2		;recupero los dos vectores
	movdqu xmm1,xmm3
	
	pslldq xmm4,2
	pslldq xmm5,2
	pand xmm0,xmm4			;xmm0 == [0,0,c,0,e,0,g,0]
	pand xmm1,xmm5			;xmm1 == [0,b,0,d,0,f,0,h]	;debe restarse!
	psrldq xmm0,2			;xmm0 == [0,c,0,e,0,g,0,-]
	psubw xmm0,xmm1			;xmm0 == [0,B,0,D,0,F,0,-]
	paddw xmm6,xmm0			;xmm7 == [A,B,C,D,E,F,G,-]
	movdqu xmm4,[todosmenos_masSig]
	pand xmm6,xmm4			;xmm7 == [A,B,C,D,E,F,G,0]

ultlineasave_mem_robY:
	packuswb xmm7, xmm6		;convierte de word a byte saturado ambos resultados y los deja en xmm7	
	movdqu [edi],xmm7

	popad
;////////////////////////////////////////////////////////////////////////
;////////////////////////////////////////////////////////////////////////
%endmacro

%macro convencion_c 0
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_c_fin 0
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
%endmacro

section .data
	acum_linex: DQ 0,0
	acum_liney: DQ 0,0
	matriz_10: DQ 0x0000ffff0000ffff, 0x0000ffff0000ffff
	matriz_01: DQ 0xffff0000ffff0000,0xffff0000ffff0000
	dejar_masSig: DQ 0, 0xffff000000000000
	todosmenos_masSig: DQ -1,0x0000ffffffffffff

section .text
	global asmRoberts

asmRoberts:
	convencion_c 
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]

	xor ecx, ecx			; ecx = 0 [contador de columnas]
	xor ebx, ebx			; ebx = 0 [contador de filas]
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	mov edi, ptr_dst		; edi = puntero al inicio de línea dst
	mov edx,width	
	dec dword height
	
cicloRob:
	robX
	robY
	
	movdqu xmm0,[acum_linex]
	movdqu xmm1,[acum_liney]
	
	paddsb xmm0,xmm1				;saturacion de X e Y/////////////////
	movdqu [edi+ecx],xmm0
;////////////////////////////////////////////////////////////////////////	
saltos:
	add ecx,15
	cmp ecx,edx
	je nueva_fila		;caso width%15 ^ %16 == 0
	
	mov eax,ecx			;caso "me qdan menos de 15 px en la linea x procesar"
	add eax,16			;ojo Q TE MOVES DE A 15, pero el 16 tiene q qdar en cero!
	cmp eax,edx
	jg ult_16px
	jmp cicloRob

ult_16px:
	ultimalinea
;/////////////////////////////////////////////////////////////////////////
;////RECORDAR Q ULTIMALINEA NO ES IGUAL A ROBX O ROBY/////////////////////
;/////////////////////////////////////////////////////////////////////////
	movdqu xmm0,[acum_linex]
	movdqu xmm1,[acum_liney]
	
	paddsb xmm0,xmm1				;saturacion de X e Y/////////////////
	movdqu [edi+ecx],xmm0
;////////////////////////////////////////////////////////////////////////	
;////////////////////////////////////////////////////////////////////////	
nueva_fila:
	xor ecx,ecx			;seteo las columnas en cero
	add esi,width		;apunto a la siguiente linea
	add edi,width		;apunto a la siguiente linea de dst a guardar
	inc ebx				;cmp si termine la imagen o no
	
	cmp ebx,height
	jne cicloRob
	
	convencion_c_fin 
