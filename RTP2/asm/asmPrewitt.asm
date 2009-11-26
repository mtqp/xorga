%include "macros.asm"
%define srcl xmm0
%define srch xmm1
%define acul xmm2
%define acuh xmm3
%define tmp1 xmm4
%define tmp2 xmm5
%define tmp3 xmm6
%define tmp4 xmm7

%macro calcularX 1
	movdqu tmp1, %1			; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 2*2		; tmp1 = c|d|e|f|g|h|0|0
	psubw tmp1, %1			; tmp1 = c-a|d-b|e-c|f-d|g-e|h-f|-g|-h
	%ifdef SAVE_RESULT		;
		movdqu tmp2, tmp1	; tmp2 = tmp1
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

	%define SAVE_RESULT
	calcularX srcl			; tmp1 = resultado de calcular la línea con los 8px menos significativos
	%undef SAVE_RESULT
	paddw acul, tmp1		; suma el resultado

	calcularX srch			; tmp1 = resultado de calcular la línea con los 8px más significativos
	paddw acuh, tmp1		; suma el resultado

; cálculo de los 2 px centrales
	movdqu tmp1, srch		; tmp1 = i|j|k|l|m|n|o|p
	pslldq tmp1, 6*2		; tmp1 = 0|0|0|0|0|0|i|j
	paddw tmp2, tmp1		; tmp2 = -|-|-|-|-|-|i-g|j-h
	psrldq tmp2, 6*2		; tmp2 = i-g|j-h|0|0|0|0|0|0
	movdqu tmp1, tmp2		; tmp1 = tmp2
	pslldq tmp2, 7*2		; tmp2 = 0|0|0|0|0|0|0|i-g
	psrldq tmp1, 1*2		; tmp1 = j-h|0|0|0|0|0|0|0
	paddw acul, tmp2		; acul += tmp2
	paddw acuh, tmp1		; acuh += tmp1
%endmacro

%macro calcularY 1
	movdqu tmp1, %1			; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 1*2		; tmp1 = b|c|d|e|f|g|h|0
	paddw tmp1, %1			; tmp1 = a+b|b+c|c+d|d+e|e+f|f+g|g+h|h
	movdqu tmp2, %1			; tmp2 = a|b|c|d|e|f|g|h
	psrldq tmp2, 2*2		; tmp2 = c|d|e|f|g|h|0|0
	paddw tmp1, tmp2		; tmp1 = a+b+c|b+c+d|c+d+e|d+e+f|e+f+g|f+g+h|g+h|h
	%ifdef SAVE_RESULT
		movdqu tmp3, tmp1	; tmp3 = tmp1
	%endif
	pslldq tmp1, 2*2		; tmp1 = 0|0|a+b+c|b+c+d|c+d+e|d+e+f|e+f+g|f+g+h
	psrldq tmp1, 1*2		; tmp1 = 0|a+b+c|b+c+d|c+d+e|d+e+f|e+f+g|f+g+h|0
%endmacro

%macro procesarLineaY 0
	pxor tmp1, tmp1			; tmp1 = 0
	movdqu srcl, [esi+ecx]		; scrl = 16px de src
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp1		; srcl = primeros 8px desempaquetados a word
	punpckhbw srch, tmp1		; srch = segundos 8px desempaquetados a word

	%define SAVE_RESULT
	calcularY srcl			; tmp1 = resultado de calcular los primeros 8px
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

	movdqu tmp1, srch		;tmp1 = i|j|k|l|m|n|o|p
	movdqu tmp2, srch		;tmp2 = tmp1
	pslldq tmp2, 1*2		;tmp2 = 0|i|-|-|-|-|-|-
	paddw tmp2, tmp1		;tmp2 = i|i+j|-|-|-|-|-|-
	psrldq tmp3, 6*2		;tmp3 = g+h|h|-|-|-|-|-|-
	paddw tmp3, tmp2		;tmp3 = g+h+i|h+i+j|-|-|-|-|-|-
	pslldq tmp3, 6*2		;tmp3 = 0|0|0|0|0|0|g+h+i|h+i+j
	movdqu tmp1, tmp3		;tmp1 = tmp3
	pslldq tmp3, 1*2		;tmp3 = 0|0|0|0|0|0|0|g+h+i
	psrldq tmp1, 7*2		;tmp1 = h+i+j|0|0|0|0|0|0|0
	%ifndef Y_LINE_3
		psubw acul, tmp3	;acul -= tmp3
		psubw acuh, tmp1	;acuh -= tmp1
	%else
		paddw acul, tmp3	;acul += tmp3
		paddw acuh, tmp1	;acuh += tmp1
	%endif

%endmacro

; calcular
%macro PrewittX 0

	procesarLineaX			; calcula los 16 px con la primer línea

	lea esi, [esi+edx]		; avanza a la siguiente línea
	procesarLineaX			; calcula los 16 px con la segunda línea

	lea esi, [esi+edx]		; avanza a la siguiente línea
	procesarLineaX			; calcular los 16 px con la tercer línea
	
	sub esi, edx			; retrocedo una línea
	sub esi, edx			; vuelvo src a la posición original
	
%endmacro

%macro PrewittY 0
	procesarLineaY			; procesa la primer línea

	lea esi, [esi+2*edx]		; avanza dos líneas
	%define Y_LINE_3
	procesarLineaY			; procesa la tercer línea
	%undef Y_LINE_3

	sub esi, edx			; retrocede una línea
	sub esi, edx			; vuelve src a la posición original
%endmacro

section .text
	global asmPrewitt

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmPrewitt:
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

	pxor tmp4, tmp4

procesar:
	pxor acul,acul			; acul = 0 [acumulador parte baja]
	pxor acuh,acuh			; acuh = 0 [acumulador parte alta]

	PrewittX
	abs acul,tmp1
	abs acuh,tmp1
	packuswb acul, acuh
	psrldq acul, 1
	movdqu tmp4, acul

procesar_y:
	pxor acul, acul
	pxor acuh, acuh
	PrewittY
	abs acul,tmp1
	abs acuh,tmp1
	packuswb acul, acuh
	psrldq acul, 1
	paddusb acul, tmp4

proximos_pixels:

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

