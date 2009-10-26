%include "macros.asm"

; procesarFragmentoX input, acumulador, shuf1, shuf2, máscara_px_procesados
; 	aplica la línea de la matríz para 2px del input y los suma en el
;	acumulador. Los px procesados son seleccionados mediante las máscaras.
%macro procesarFragmentoX 3
	movdqu xmm2, %1			; copio el input original
	pmullw xmm2,xmm5		; xmm2 = [8px]*[línea de la matríz]
					; [a,B,c,d,E,f,o,o]
					; b<-a+b+c,E<-d+e+f (b y e son ceros!)
	psrldq xmm2, (%3-1)*2		; xmm2 = [x1,-,y1,x2,-,y2,-,-]
	movdqu xmm3, xmm2
	psrldq xmm3, 4			; xmm3 = [y1,-,- ,y2,-,- ,-,-]
	;pshuflw xmm3, xmm3, 00110000b	; xmm3 = [y1,-,y2,- ,-,- ,-,-]
	paddw xmm2, xmm3		; xmm2 = [x1+y1,-,-,x2+y2,-.-.-.-]
	movdqu xmm4, [mask_limpiar]
	andps xmm2, xmm4		; xmm2 = [x1+y1,0,0,x2+y2,0,0,0,0] 
	
	;pshufd xmm3, xmm2, %3		; 
	;pshuflw xmm3,xmm3, %4

	;paddw xmm2,xmm3			; xmm2 == [a+c,-,-,d+f,-,-,-,-]
	;pslldq xmm2,2			; xmm2 == [-,a+c,-,-,d+f,-,-,-]
	;movdqu xmm4,[%5]
	;andps xmm2, xmm4		; xmm2 == [0,a+c,0,0,d+f,0,0,0]
	pslldq xmm2,%3*2
	paddw %2, xmm2			; xmm7 += xmm2
%endmacro

; procesarLineaX input, output, línea_matríz
;	aplica la línea de la matríz a los 6 px centrales del input
;	El resultado se acumula en output. Tanto input como output
;	deben ser 8 words.
%macro procesarLineaX 3
	movdqu xmm5, [%3]		; xmm5 = línea de la matríz a aplicar

	;procesarFragmentoX %1, %2, 00001001b, 11100100b, mask_borrar_px1
	procesarFragmentoX %1, %2, 1

	pslldq xmm5,2			; xmm5 == [0,-1,0,1,-1,0,1,-1]
	;procesarFragmentoX %1, %2, 00001100b, 00000010b, mask_borrar_px2
	procesarFragmentoX %1, %2, 2

	pslldq xmm5,2			; xmm5 == [0,0,-1,0,1,-1,0,1]
	procesarFragmentoX %1, %2, 3
%endmacro

%macro aplicarLineaX 2			; parámetros: puntero a src, línea de la matriz | resultado en xmm7
	pushad				; guardo todos los registros de propósito general
	mov esi, %1			; esi = puntero a src

	movdqu xmm0,[esi]		; xmm0 = 16 pixels de src
	movdqu xmm1, xmm0

	pxor xmm5, xmm5			; xmm5 = 0
	punpcklbw xmm0,xmm5		; xmm0 = primeros ocho px convertidos a word
	punpckhbw xmm1,xmm5		; xmm1 = segundos ocho px convertidos a word

; proceso la parte baja
	procesarLineaX xmm0, xmm7, %2

; proceso los pixeles del medio
	movdqu xmm5, [%2]			; xmm5 = [m1,m2,m3,m1,m2,m3,m1,m2] (línea de la matríz)
	movdqu xmm2, xmm0			; xmm2 = [a,b,c,d,e,f,g,h] (primeros 8 px)
	pmullw xmm2, xmm5			; xmm2 = primeros 8 px multiplicados por la matríz
	movdqu xmm4, xmm5			; xmm4 = línea de la matríz
	psrldq xmm4, 4				; xmm4 = [m3,m1,m2,m3,m1,m2,0,0]
	movdqu xmm3, xmm1			; xmm3 = [i,j,k,l,m,n,o,p] (segundos 8 px)
	pmullw xmm3, xmm4			; xmm3 = segundos 8 px multiplicados por la máscara corrida
	pshufd xmm3, xmm3, 00000000b		; xmm3 = [x,x,x,x,x,x,i*m3,x]
	movdqu xmm4, [mask_2words_masSig]	; xmm4 = 0xFFFF0...0
	andps xmm3, xmm4			; xmm3 = [0,0,0,0,0,0,i*m3,x]
	andps xmm2, xmm4			; xmm2 = [0,0,0,0,0,0,g*m1,h*m2]
	paddw xmm3, xmm2			; xmm3 = [0,0,0,0,0,0,i*m3+g*m1,x]
	pshufhw xmm2, xmm2, 11000000b
	pshufhw xmm3, xmm3, 10000000b		; xmm3 = [0,0,0,0,0,0,0,i*m3+g*m1]
	paddw xmm3, xmm2			; xmm3 = [0,0,0,0,0,0,0,i*m3+g*m1+h*m2]
	paddw xmm7, xmm3			; acumulo en xmm7 el resultado

	movdqu xmm2, xmm0			; xmm2 = [a,b,c,d,e,f,g,h] (primeros 8px)
	movdqu xmm3, xmm1			; xmm1 = [i,j,k,l,m,n,o,p] (segundos 8px)
	movdqu xmm4, xmm5			; xmm4 = línea de la matríz
	psrldq xmm4, 1				; xmm4 = [0,m1,m2,m3,m1,m2,m3,m1]
	pmullw xmm2, xmm4			; xmm2 = [0,x ,x ,x ,x ,x ,x ,h*m1]
	pslldq xmm4, 2				; xmm4 = [m2,m3,m1,m2,m3,m1,0,0]
	pmullw xmm3, xmm4			; xmm3 = [i*m2,j*m3,x,x,x,x,x,x]
	movdqu xmm4, [mask_2words_menSig]	; xmm4 = 0x0...0FFFF
	andps xmm3, xmm4			; xmm3 = [i*m2,j*m3,0,0,0,0,0,0]
	movdqu xmm4, [mask_2words_masSig]	; xmm4 = 0xFFFF0...0
	andps xmm2, xmm4			; xmm2 = [0,0,0,0,0,0,x,h*m1]
	pshufd xmm2, xmm2, 00000011b		; xmm2 = [x,h*m1,0,0,0,0,0,0]
	pshuflw xmm2, xmm2, 11111101b		; xmm2 = [h*m1,0,0,0,0,0,0,0]
	paddw xmm3, xmm2			; xmm3 = [i*m2+h*m1,j*m3,0,0,0,0,0,0]
	pshuflw xmm2, xmm3, 11111101b		; xmm2 = [j*m3,0,0,0,0,0,0,0]
	paddw xmm3, xmm2			; xmm3 = [i*m2+h*m1+j*m3,0,0,0,0,0,0]
	;paddw xmm6, xmm3			; acumulo en xmm6 el resultado
	

	;movdqu xmm4,[mask_2words_masSig]	; xmm4 = máscara para dejar las 2 words más significativas
	;andps xmm0, xmm4			; xmm0 == [0,0,0,0,0,0,g,h]
	;movdqu xmm3,xmm1			; xmm3 == [i,j,k,l,m,n,o,p]
	;movdqu xmm4,[mask_2words_menSig]	; xmm4 = máscara para dejar las 2 words menos significativas
	;andps xmm3, xmm4			; xmm3 == [i,j,0,0,0,0,0,0]


	;pshuflw xmm3, xmm3, 01001100b		; xmm3 == [i,0,i,j,0,0,0,0]
	;pshufd xmm3, xmm3, 01110011b		; xmm3 == [0,0,i,0,0,0,i,j]
	;pshufhw xmm3, xmm3, 00111000b		; xmm3 == [0,0,i,0,0,i,j,0]
	;pshufd xmm0, xmm0, 00110011b		; xmm0 == [g,h,0,0,g,h,0,0]
	;pshufhw xmm0, xmm0, 11101001b		; xmm0 == [g,h,0,0,h,0,0,0]
	;paddw xmm0, xmm3			; xmm0 == [g,h,i,0,h,i,j,0]
	;movdqu xmm4,[sobel_X_medio]		; xmm4 == mascara del medio
	;pmaddwd xmm0, xmm4			; xmm0 == [g*m1+h*m2,i*m3,h*m5+i*m6,j*m7]
	;pshufd xmm5, xmm0, 00110001b		; xmm5 == [i*m3,X,j*m7,X]
	;paddw xmm0, xmm5			; xmm0 == [g*m1+h*m2+i*m3, X, h*m5+i*m6+j*m7, X]
	;packusdw xmm5, xmm5			; xmm5 == [res1,X,res2,X,res1,X,res2,X]
	;pxor xmm3, xmm3	
	;pshufhw xmm3, xmm5, 00000000b		; xmm3 == [0,0,0,0,res1,res1,res1,res1]
	;pshufd xmm3, xmm3, 11000000b		; xmm3 == [0,0,0,0,0,0,res1,res1]
	;pshufhw xmm3, xmm3, 11000000b		; xmm3 == [0,0,0,0,0,0,0,res1]
	;paddw xmm7, xmm3
	

	;paddw xmm0,xmm3				; xmm0 == [i,j,0,0,0,0,g,h]
	;pshufd xmm0,xmm0,00111100b		; xmm0 == [i,j,g,h,g,h,i,j]
	;pshuflw xmm3,xmm0,00001100b		; xmm3 == [++,-h,+,+,+,+,-g,++]
	;pshufhw xmm3,xmm3,00000000b
	;movdqu xmm4,[neg_1]
	;pmullw xmm3,xmm4
	;paddw xmm0,xmm3			;xmm0 == [- ,AH,-,-,--,-,HH, -]
	;pshuflw xmm0,xmm0,00000001b
	;pshufhw xmm0,xmm0,10000000b
	;movdqu xmm4,[mask_primYult]
	;andps xmm0,xmm4			;xmm0 == [AH,-,-,-,-,-,-,-,HH]
	;movdqu xmm5,xmm0
	;movdqu xmm4,[mask_2words_menSig]
	;andps xmm5,xmm4			;xmm6 == [aH,0,0,0,0,0,0,0]
	;movdqu xmm4,[mask_2words_masSig]
	;andps xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,h]
	;paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,h]

; proceso la parte alta
	procesarLineaX xmm1, xmm6, %2
	
	popad				; vuelvo todos los registros a como estaban antes de llamar a la macro
%endmacro

section .data
	acum_lineas : DQ 0,0,0,0
	acum_filas : DD 0
	sobel_X_linea1: DW -1,0,1,-1,0,1,-1,0
	sobel_X_linea2: DW -2,0,2,-2,0,2,-2,0
	sobel_X_medio: DW -1,0,1,-1,0,1,0
	mask_borrar_px1: DQ 0x00000000ffff0000, 0x000000000000ffff
	mask_borrar_px2: DQ 0x0000ffff00000000, 0x00000000ffff0000
	mask_borrar_px3: DQ 0xffff000000000000, 0x0000ffff00000000
	mask_limpiar: 	 DQ 0xFFFF00000000FFFF, 0x0000000000000000
	neg_1: DQ -1,-1
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
	mask_primYult: DQ 0x000000000000ffff,0xffff000000000000
section .text
	global asmSobel

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmSobel:
	convencion_C		
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]
	%define xorder		[ebp+24]
	%define yorder 		[ebp+28]
	
	xor ecx, ecx			; ecx = 0 [contador de columnas]
	xor ebx, ebx			; ebx = 0 [contador de filas]
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	mov edi, ptr_dst		; edi = puntero al inicio de línea dst

	dec dword height
	add edi,width			; avanza edi una línea xq la 1er línea va en negro
	mov eax,xorder			; eax = xorder (nunca ambos tienen q ser cero!)
	mov ebx,yorder			; eax = yorder (xq x implement haria el deriv_xy)
	cmp eax,ebx			; compara xorder con yorder
;	je deriv_xy			; si son iguales, entonces es porque se usan ambas derivadas (nunca deberia pasarse como param 0, 0)
;	jg deriv_y			; sino, si sólo tiene yorder, salta la parte de xorder
procesar:

ciclo_xorder:
	pxor xmm7,xmm7			; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6			; xmm6 = 0 [acumulador parte alta]

	lea edx, [esi+ecx]
	aplicarLineaX edx, sobel_X_linea1	; calcula en xmm6:xmm7 la primer línea
db:
	add edx, width
	aplicarLineaX edx, sobel_X_linea2
	add edx, width
	aplicarLineaX edx, sobel_X_linea1

	packuswb xmm7, xmm6		; convierte de word a byte saturado ambos resultados y los deja en xmm7
	movdqu [edi+ecx], xmm7		; copia el resultado a memoria

;//////////////////////////////////////////
	add ecx,14			; avanza 14 columnas
					; verifica si está cerca del final de la línea
	mov edx, ecx			; edx = columna actual (temporalmente)
	add edx, 14			; edx = columna actual + 14
	cmp edx, width			; verifica si al procesar 14 bytes se pasa o no del ancho
	jl ciclo_xorder			; si no se pasa continúa procesando
					; si se pasa..
	cmp ecx,width			; compara la columna actual con el ancho de la imágen
	je avanzar_linea		; si son iguales avanza a la siguiente fila
					; sino reposiciona la columna para terminar de procesar
	mov edx, width			; edx = ancho
	sub edx, 14			; edx = ancho - 14 [posible segfault! lo dejo porque anda por ahora]
	mov ecx, edx			; columna actual = ancho - 14
	jmp ciclo_xorder		; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	add edi, width			; avanza la fila correspondiente
	add esi, width
	inc ebx				; incrementa el contador de filas
	cmp ebx, height			; compara el contador de filas con la altura
	jne ciclo_xorder		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

