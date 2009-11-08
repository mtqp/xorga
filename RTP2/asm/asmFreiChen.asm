%include "macros.asm"
%define srcl xmm0
%define srch xmm1
%define acul xmm2
%define acuh xmm3
%define tmp1 xmm4
%define tmp2 xmm5
%define tmp3 xmm6
%define sqr2 xmm7

%define tmp4 xmm0			; se usa sólo después de tener la parte baja calculada

; CalcularX src
;	Calcula los 6px del medio de src aplicando 
;	la línea en X seleccionada
;	-Parámetros:
;		src	8px desempaquetados en word
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
	pslldq tmp1, 2*2		; tmp1 = 0|0|R*(c-a)|R*(d-b)|R*(e-c)|R*(f-d)|R*(g-e)|R*(h-f)
	psrldq tmp1, 1*2		; tmp1 = 0|R*(c-a)|R*(d-b)|R*(e-c)|R*(f-d)|R*(g-e)|R*(h-f)|0
%endmacro

; procesarLineaX
;	Calcula los 14px a partir de esi+ecx+1 y 
;	suma el resultado en los acumuladores
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

%macro calcularY 1
	movdqu tmp1, %1			; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 1*2		; tmp1 = b|c|d|e|f|g|h|0
; multiplico por sqrt(2)
		pxor tmp3, tmp3 	; tmp3 = 0
		movdqu tmp2, tmp1	; tmp2 = tmp1
		pcmpgtw tmp3, tmp2	; tmp3 = extensión de signo de tmp1/tmp2
		punpckhwd tmp2, tmp3	; tmp2 = e|f|g|h
		punpcklwd tmp1, tmp3	; tmp1 = a|b|c|d
		cvtdq2ps tmp1, tmp1	; tmp1 = (float)tmp1
		cvtdq2ps tmp2, tmp2	; tmp2 = (float)tmp2
		mulps tmp1, sqr2	; tmp2 = R*e|R*f|R*g|R*h
		mulps tmp2, sqr2	; tmp1 = R*a|R*b|R*c|R*d
		cvtps2dq tmp1, tmp1	; tmp1 = (int)tmp1
		cvtps2dq tmp2, tmp2	; tmp2 = (int)tmp2
		packssdw tmp1, tmp2	; tmp1 = (short)tmp1|(short)tmp2
; sigue
	paddw tmp1, %1			; tmp1 = a+R*b|b+R*c|c+R*d|d+R*e|e+R*f|f+R*g|g+R*h|h
	movdqu tmp2, %1			; tmp2 = a|b|c|d|e|f|g|h
	psrldq tmp2, 2*2		; tmp2 = c|d|e|f|g|h|0|0
	paddw tmp1, tmp2		; tmp1 = a+R*b+c|b+R*c+d|c+R*d+e|d+R*e+f|e+R*f+g|f+R*g+h|g+R*h|h
	%ifdef SAVE_RESULT
		movdqu tmp4, tmp1	; tmp4 = tmp1
	%endif
	pslldq tmp1, 2*2		; tmp1 = 0|0|a+R*b+c|b+R*c+d|c+R*d+e|d+R*e+f|e+R*f+g|f+R*g+h
	psrldq tmp1, 1*2		; tmp1 = 0|a+R*b+c|b+R*c+d|c+R*d+e|d+R*e+f|e+R*f+g|f+R*g+h|0
%endmacro

%macro procesarLineaY 0
	pxor tmp1, tmp1			; tmp1 = 0
	movdqu srcl, [esi+ecx]		; srcl = 16px de src
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp1		; srcl = primeros 8px desempaquetados a word
	punpckhbw srch, tmp1		; srch = segundos 8px desempaquetados a word

	%define SAVE_RESULT
	calcularY srcl			; tmp1 = resultado de calcular los primeros 8px
					; no necesito más srcl, a partir de acá es tmp4
	%undef SAVE_RESULT
	%ifndef Y_LINE_3
		psubw acul, tmp1	; si es la línea 1 resta el resultado al acumulador
	%else
		paddw acul, tmp1	; si es la línea 3 suma el resultado al acumulador
	%endif
	
	calcularY srch			; tmp1 = resultado de calcular los segundos 8px
	%ifndef Y_LINE_3
		psubw acuh, tmp1	; si es la línea 1 resta el resultado al acumulador
	%else
		paddw acuh, tmp1	; si es la línea 3 suma el resultado al acumulador
	%endif

; calcula los 2px del medio
	movdqu tmp1, srch		;tmp1 = i|j|k|l|m|n|o|p
	movdqu tmp2, srch		;tmp2 = tmp1
	; calcula R*j
		pxor tmp3, tmp3 	; tmp3 = 0
		pcmpgtw tmp3, tmp2	; tmp3 = extensión de signo de tmp2
		punpcklwd tmp2, tmp3	; tmp2 = i|-|-|-
		cvtdq2ps tmp2, tmp2	; tmp2 = (float)tmp2
		mulps tmp2, sqr2	; tmp2 *= sqrt(2)
		cvtps2dq tmp2, tmp2	; tmp2 = (int)tmp2
		packssdw tmp2, tmp2	; tmp2 = (short)tmp2|(short)tmp2 = R*i|-|-|-|-|-|-|-
	; sigue
	pslldq tmp2, 1*2		;tmp2 = 0|R*i|-|-|-|-|-|-
	paddw tmp2, tmp1		;tmp2 = i|R*i+j|-|-|-|-|-|-
	psrldq tmp4, 6*2		;tmp4 = g+R*h|h|-|-|-|-|-|-
	paddw tmp4, tmp2		;tmp4 = g+R*h+i|h+R*i+j|-|-|-|-|-|-
	pslldq tmp4, 6*2		;tmp4 = 0|0|0|0|0|0|g+R*h+i|h+R*i+j
	movdqu tmp3, tmp4		;tmp3 = tmp4
	pslldq tmp4, 1*2		;tmp4 = 0|0|0|0|0|0|0|g+R*h+i
	psrldq tmp3, 7*2		;tmp3 = h+R*i+j|0|0|0|0|0|0|0
	%ifndef Y_LINE_3
		psubw acul, tmp4	;acul -= tmp4
		psubw acuh, tmp3	;acuh -= tmp3
	%else
		paddw acul, tmp4	;acul += tmp4
		paddw acuh, tmp3	;acuh += tmp3
	%endif

%endmacro

%macro FreiChenY 0
	procesarLineaY

	lea esi, [esi+2*edx]
	%define Y_LINE_3
	procesarLineaY
	%undef Y_LINE_3

	sub esi, edx			; retrocedo una línea
	sub esi, edx			; vuelvo src a la posición original
%endmacro


section .data
	dos: DD 2.0,2.0,2.0,2.0

section .text
	global asmFreiChen

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

	FreiChenY
	abs acul
	abs acuh

	packuswb acul, acuh		; acul = 0|B|C|D|E|F|G|H|I|J|K|L|M|N|O|0
	psrldq acul, 1			; acul = B|C|D|E|F|G|H|I|J|K|L|M|N|O|0|0
	movdqu [edi+ecx+1], acul	; copia el resultado a memoria

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

