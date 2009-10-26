%include "macros.asm"

section .data
	prewitt_X: DW -1,0,1,-1,0,1,-1,0
	prewitt_Y_linea1: DW -1,-1,-1,-1,-1,-1,-1,-1
	prewitt_Y_linea3: DW 1,1,1,1,1,1,1,1
	mask_limpiar: 	 DQ 0xFFFF00000000FFFF, 0x0000000000000000
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
section .text
	global asmPrewitt

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmPrewitt:
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
	aplicarLineaX edx, prewitt_X		; calcula en xmm6:xmm7 la primer línea
	add edx, width
	aplicarLineaX edx, prewitt_X
	add edx, width
	aplicarLineaX edx, prewitt_X

	abs xmm7
	abs xmm6
	packuswb xmm7, xmm6			; convierte de word a byte saturado ambos resultados y los deja en xmm7
	psrldq xmm7, 1
	movdqu [acum], xmm7

	pxor xmm7,xmm7				; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6				; xmm6 = 0 [acumulador parte alta]

	cmp ah, 0
	je proximos_pixels

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

