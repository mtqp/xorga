%include "macros.asm"

; float_procesarFragmentoX input, acumulador, nro de par
; 	aplica la línea de la matríz para 2px del input y los suma en el
;	acumulador. Los px procesados son seleccionados mediante las máscaras.
%macro float_procesarFragmentoX 3
	movdqu xmm2, %1			; copio el input original
	pmullw xmm2,xmm5		; xmm2 = [8px]*[línea de la matríz]
					; [a,B,c,d,E,f,o,o]
					; b<-a+b+c,E<-d+e+f (b y e son ceros!)
	psrldq xmm2, (%3-1)*2		; xmm2 = [x1,z1,y1,x2,z2,y2,-,-]
	movdqu xmm3, xmm2
	movdqu xmm4, xmm2
	psrldq xmm4, 2			; xmm4 = [z1,-,-,z2,-,-,-,-]
	paddw xmm2, xmm4		; xmm2 = [x1+z1,-,-,x2+z2,-,-,-]
	psrldq xmm3, 4			; xmm3 = [y1,-,- ,y2,-,- ,-,-]
	paddw xmm2, xmm3		; xmm2 = [x1+z1+y1,-,-,x2+z2+y2,-.-.-.-]
	movdqu xmm4, [mask_limpiar]
	andps xmm2, xmm4		; xmm2 = [x1+z2+y1,0,0,x2+z2+y2,0,0,0,0] 
	
	pslldq xmm2,%3*2
	paddw %2, xmm2			; xmm7 += xmm2
%endmacro

; procesarLineaX input, output, línea_matríz
;	aplica la línea de la matríz a los 6 px centrales del input
;	El resultado se acumula en output. Tanto input como output
;	deben ser 8 words.
%macro procesarLineaX 3
	movdqu xmm5, [%3]		; xmm5 = línea de la matríz a aplicar

	procesarFragmentoX %1, %2, 1

	pslldq xmm5,2			; xmm5 == [0,-1,0,1,-1,0,1,-1]
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
	pslldq xmm4, 2				; xmm4 = [0,m1,m2,m3,m1,m2,m3,m1]
	pmullw xmm2, xmm4			; xmm2 = [0,x ,x ,x ,x ,x ,x ,h*m1]
	psrldq xmm4, 4				; xmm4 = [m2,m3,m1,m2,m3,m1,0,0]
	pmullw xmm3, xmm4			; xmm3 = [i*m2,j*m3,x,x,x,x,x,x]
	movdqu xmm4, [mask_2words_menSig]	; xmm4 = 0x0...0FFFF
	andps xmm3, xmm4			; xmm3 = [i*m2,j*m3,0,0,0,0,0,0]
	movdqu xmm4, [mask_2words_masSig]	; xmm4 = 0xFFFF0...0
	andps xmm2, xmm4			; xmm2 = [0,0,0,0,0,0,x,h*m1]
	pshufd xmm2, xmm2, 00000011b		; xmm2 = [x,h*m1,0,0,0,0,0,0]
	pshuflw xmm2, xmm2, 11111101b		; xmm2 = [h*m1,0,0,0,0,0,0,0]
	paddw xmm3, xmm2			; xmm3 = [i*m2+h*m1,j*m3,0,0,0,0,0,0]
	pshuflw xmm2, xmm3, 11111101b		; xmm2 = [j*m3,0,0,0,0,0,0,0]
	paddw xmm3, xmm2			; xmm3 = [i*m2+h*m1+j*m3,j*m3,0,0,0,0,0]
	pshuflw xmm3, xmm3, 11111100b		; xmm3 = [i*m2+h*m1+j*m3,0,0,0,0,0,0]
	paddw xmm6, xmm3			; acumulo en xmm6 el resultado
	
; proceso la parte alta
	procesarLineaX xmm1, xmm6, %2
	
	popad				; vuelvo todos los registros a como estaban antes de llamar a la macro
%endmacro



section .data
	freiChen_X_linea1: DD -1.0,   0, 1.0, 0
	freiChen_X_linea2: DD  2.0,   0, 2.0, 0
	freiChen_Y_linea1: DD  1.0, 2.0, 1.0, 0
	freiChen_Y_linea3: DD  1.0, 2.0, 1.0, 0
	mask_limpiar: 	 DQ 0xFFFF00000000FFFF, 0x0000000000000000
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
section .text
	global asmPrewitt

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmFreiChen:
	convencion_C_mem 16		
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]
	%define xorder		[ebp+24]
	%define yorder 		[ebp+28]
	%define acum		ebp-20

	xor ecx, ecx			; ecx = 0 [contador de columnas]
	xor ebx, ebx			; ebx = 0 [contador de filas]
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	mov edi, ptr_dst		; edi = puntero al inicio de línea dst

	sub dword height, 3
	add edi,width			; avanza edi una línea xq la 1er línea va en negro
	mov al,xorder			; eax = xorder (nunca ambos tienen q ser cero!)
	mov ah,yorder			; eax = yorder (xq x implement haria el deriv_xy)
procesar:

	pxor xmm7,xmm7				; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6				; xmm6 = 0 [acumulador parte alta]
	movdqu [acum],xmm7
	
	cmp al, 0
	je derivada_y

	lea edx, [esi+ecx]
	float_aplicarLineaX edx, prewitt_X		; calcula en xmm6:xmm7 la primer línea
	;add edx, width
	;float_aplicarLineaX edx, prewitt_X
	;add edx, width
	;aplicarLineaX edx, prewitt_X

	abs xmm7
	abs xmm6
	packuswb xmm7, xmm6			; convierte de word a byte saturado ambos resultados y los deja en xmm7
	psrldq xmm7, 1
	movdqu [acum], xmm7

	pxor xmm7,xmm7				; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6				; xmm6 = 0 [acumulador parte alta]

	cmp ah, 0
	jmp proximos_pixels

derivada_y:
	lea edx, [esi+ecx]
	aplicarLineaX edx, prewitt_Y_linea1	; calcula en xmm6:xmm7 la primer línea
	add edx, width
	add edx, width
	aplicarLineaX edx, prewitt_Y_linea3
	abs xmm6
	abs xmm7
	packuswb xmm7, xmm6			; convierte de word a byte saturado ambos resultados y los deja en xmm7
	psrldq xmm7, 1

proximos_pixels:
	movdqu xmm6, [acum]
	paddusb xmm6, xmm7
	movdqu [edi+ecx+1], xmm6		; copia el resultado a memoria

;//////////////////////////////////////////
	add ecx,14			; avanza 14 columnas
					; verifica si está cerca del final de la línea
	mov edx, ecx			; edx = columna actual (temporalmente)
	add edx, 14			; edx = columna actual + 14
	cmp edx, width			; verifica si al procesar 14 bytes se pasa o no del ancho
	jl procesar			; si no se pasa continúa procesando
					; si se pasa..
	cmp ecx,width			; compara la columna actual con el ancho de la imágen
	je avanzar_linea		; si son iguales avanza a la siguiente fila
					; sino reposiciona la columna para terminar de procesar
	mov edx, width			; edx = ancho
	sub edx, 14			; edx = ancho - 14 [posible segfault! lo dejo porque anda por ahora]
	mov ecx, edx			; columna actual = ancho - 14
	jmp procesar			; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	add edi, width			; avanza la fila correspondiente
	add esi, width
	inc ebx				; incrementa el contador de filas
	cmp ebx, height			; compara el contador de filas con la altura
	jne procesar	 		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin_mem 16


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

