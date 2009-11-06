%include "macros.asm"
%define srcl xmm0
%define srch xmm1
%define acul xmm2
%define acuh xmm3
%define tmp1 xmm4
%define tmp2 xmm5
%define tmp3 xmm6
%define sqr2 xmm7

%macro calcularX 1
	movdqu tmp1, %1			; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 2*2		; tmp1 = c|d|e|f|g|h|0|0
	psubw tmp1, %1			; tmp1 = c-a|d-b|e-c|f-d|g-e|h-f|-g|-h
	%ifdef X_LINE_2
		pxor tmp3, tmp3 	; tmp3 = 0
		movdqu tmp2, tmp1	; tmp2 = tmp1
		pcmpgtw tmp3, tmp2	; tmp3 = extensión de signo de tmp2
		punpckhwd tmp2, tmp3	; tmp2 = g-e|h-f|-g |-h
		punpcklwd tmp1, tmp3	; tmp1 = c-a|d-b|e-c|f-d
		cvtdq2ps tmp1, tmp1	; tmp1 = (float)tmp1
		cvtdq2ps tmp2, tmp2	; tmp2 = (float)tmp2
		mulps tmp1, sqr2	; tmp2 = R*(g-e)|R*(h-f)|r*(-g)|R*(-h)
		mulps tmp2, sqr2	; tmp1 = R*(c-a)|R*(d-b)|R*(e-c)|R*(f-d)
		cvtps2dq tmp1, tmp1	; tmp1 = (int)tmp1
		cvtps2dq tmp2, tmp2	; tmp2 = (int)tmp2
		packssdw tmp1, tmp2	; tmp1 = (short)tmp1|(short)tmp2
	%endif
	pslldq tmp1, 2*2		; tmp1 = 0|0|c-a|d-b|e-c|f-d|g-e|h-f
	psrldq tmp1, 1*2		; tmp1 = 0|c-a|d-b|e-c|f-d|g-e|h-f|0
%endmacro

%macro procesarLineaX 0
	pxor tmp1, tmp1			; tmp1 = 0
	movdqu srcl, [esi+ecx]		; srcl = 16px
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp1		; srcl = a|b|c|d|e|f|g|h
	punpckhbw srch, tmp1		; srch = i|j|k|l|m|n|o|p

	calcularX srcl			; tmp1 = resultado de calcular la línea con los 8px menos significativos
	paddw acul, tmp1		; suma el resultado

; calcula los 2 px centrales
	movdqu tmp2, srcl		; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp2, 2*2		; tmp1 = c|d|e|f|g|h|0|0
	psubw tmp2, srcl		; tmp1 = c-a|d-b|e-c|f-d|g-e|h-f|-g|-h
	movdqu tmp1, srch		; tmp1 = i|j|k|l|m|n|o|p
	pslldq tmp1, 6*2		; tmp1 = 0|0|0|0|0|0|i|j
	paddw tmp2, tmp1		; tmp2 = -|-|-|-|-|-|i-g|j-h
	%ifdef X_LINE_2
		pxor tmp3, tmp3 	; tmp3 = 0
		pcmpgtw tmp3, tmp2	; tmp3 = extensión de signo de tmp2
		punpckhwd tmp2, tmp3	; tmp2 = -|-|i-g|j-h
		cvtdq2ps tmp2, tmp2	; tmp2 = (float)tmp2
		mulps tmp2, sqr2	; tmp2 *= sqrt(2)
		cvtps2dq tmp2, tmp2	; tmp2 = (int)tmp2
		packssdw tmp2, tmp2	; tmp2 = (short)tmp2|(short)tmp2
	%endif
	psrldq tmp2, 6*2		; tmp2 = i-g|j-h|0|0|0|0|0|0
	movdqu tmp1, tmp2		; tmp1 = tmp2
	pslldq tmp2, 7*2		; tmp2 = 0|0|0|0|0|0|0|i-g
	psrldq tmp1, 1*2		; tmp1 = j-h|0|0|0|0|0|0|0

	paddw acul, tmp2		; acul += tmp2
	paddw acuh, tmp1		; acuh += tmp1

; calcula los 8px más significativos
	calcularX srch			; tmp1 = resultado de calcular la línea con los 8px más significativos
	paddw acuh, tmp1		; suma el resultado

%endmacro

; calcular
%macro FreiChenX 0

	procesarLineaX			; calcula los 16 px con la primer línea

	lea esi, [esi+edx]		; avanza a la siguiente línea
	%define X_LINE_2
	procesarLineaX			; calcula los 16 px con la segunda línea
	%undef X_LINE_2

	lea esi, [esi+edx]		; avanza a la siguiente línea
	procesarLineaX			; calcular los 16 px con la tercer línea
	
	sub esi, edx			; retrocedo una línea
	sub esi, edx			; vuelvo src a la posición original
	
%endmacro

section .data
	dos: DD 2.0,2.0,2.0,2.0

section .text
	global asmFreiChen

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmFreiChen:
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
	mov edx, width

	sub dword height, 3
	add edi,width			; avanza edi una línea xq la 1er línea va en negro
	mov al,xorder			; eax = xorder
	mov ah,yorder			; eax = yorder
		
	movdqu sqr2, [dos]		; sqr2 = 2|2|2|2
	sqrtps sqr2, sqr2		; sqr2 = R|R|R|R  R = sqrt(2)

procesar:
	pxor acul,acul			; acul = 0 [acumulador parte baja]
	pxor acuh,acuh			; acuh = 0 [acumulador parte alta]

	FreiChenX
	abs acul
	abs acuh

	;FreiChenY
	;abs acul
	;abs acuh

	packuswb acul, acuh
	psrldq acul, 1
	movdqu [edi+ecx+1], acul

	lea ecx, [ecx+2*16-2*2]		; avanzo 28 columnas
	cmp ecx, edx			; verifica si al procesar 14 bytes se pasa o no del ancho
	lea ecx, [ecx-14]		; columna actual = columna del ciclo anterior + 14
	jl procesar			; si no se pasa continúa procesando
					; si se pasa..

	cmp ecx, edx			; compara la columna actual con el ancho de la imágen
	je avanzar_linea		; si son iguales avanza a la siguiente fila
					; sino reposiciona la columna para terminar de procesar
	lea ecx, [edx-14]		; columna actual = ancho - 14
	jmp procesar			; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	lea edi, [edi+edx]		; avanza la fila correspondiente
	lea esi, [esi+edx]
	inc ebx				; incrementa el contador de filas
	cmp ebx, height			; compara el contador de filas con la altura
	jne procesar	 		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin

