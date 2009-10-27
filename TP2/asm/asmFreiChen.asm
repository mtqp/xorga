%include "macros.asm"

section .data
	freiChen_X_linea1: DD -1.0,   0, 1.0, 0
	freiChen_X_linea2: DD  2.0,   0, 2.0, 0
	freiChen_Y_linea1: DD  1.0, 2.0, 1.0, 0
	freiChen_Y_linea3: DD  1.0, 2.0, 1.0, 0
	mask_limpiar: 	 DQ 0xFFFF00000000FFFF, 0x0000000000000000
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
section .text
	global asmFreiChen

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
	mov ecx,width			; ecx = ancho imagen
	add edi,width			; avanza edi una línea xq la 1er línea va en negro
	dec height				; la ultima linea NO se procesa
	
;//////////////////////////////////////////
	add ecx,14			; avanza 14 columnas
					; verifica si está cerca del final de la línea
	mov edx, ecx			; edx = columna actual (temporalmente)
	add edx, 14			; edx = columna actual + 14
	cmp edx, width			; verifica si al procesar 14 bytes se pasa o no del ancho
;	jl procesar			; si no se pasa continúa procesando
					; si se pasa..
	cmp ecx,width			; compara la columna actual con el ancho de la imágen
	je avanzar_linea		; si son iguales avanza a la siguiente fila
					; sino reposiciona la columna para terminar de procesar
	mov edx, width			; edx = ancho
	sub edx, 14			; edx = ancho - 14 [posible segfault! lo dejo porque anda por ahora]
	mov ecx, edx			; columna actual = ancho - 14
;	jmp procesar			; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	add edi, width			; avanza la fila correspondiente
	add esi, width
	inc ebx				; incrementa el contador de filas
	cmp ebx, height			; compara el contador de filas con la altura
;	jne procesar	 		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin_mem 16


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

