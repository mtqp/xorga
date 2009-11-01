%include "macros.asm"
%define srcl xmm0
%define srch xmm1
%define acul xmm2
%define acuh xmm3
%define tmp1 xmm4
%define tmp2 xmm5
%define tmp3 xmm6
%define tmp4 xmm7

; calcular
%macro RobertsX 0

	movdqu srcl, [esi+ecx]		; srcl = 16px
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp3		; srcl = a|b|c|d|e|f|g|h
	punpckhbw srch, tmp3		; srch = i|j|k|l|m|n|o|p
	paddw acul, srcl
	paddw acuh, srch
	
	lea esi, [esi+edx]		; avanza a la siguiente línea
	movdqu srcl, [esi+ecx]		; srcl = 16px
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp3		; srcl = a|b|c|d|e|f|g|h
	punpckhbw srch, tmp3		; srch = i|j|k|l|m|n|o|p
	
	movdqu tmp1, srcl		; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 1*2		; tmp1 = b|c|d|e|f|g|h|0
	psubw acul, tmp1		; acul = (a)-b|(b)-c|(c)-d|(d)-e|...
	movdqu tmp1, srch
	pslldq tmp1, 7*2
	psubw acul, tmp1
	movdqu tmp1, srch
	psrldq tmp1, 1*2
	psubw acuh, tmp1
	sub esi, edx			; vuelvo src a la posición original
	
%endmacro

%macro RobertsY 0

	movdqu srcl, [esi+ecx]		; srcl = 16px
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp3		; srcl = a|b|c|d|e|f|g|h
	punpckhbw srch, tmp3		; srch = i|j|k|l|m|n|o|p
	movdqu tmp1, srcl		; tmp1 = a|b|c|d|e|f|g|h
	psrldq tmp1, 1*2		; tmp1 = b|c|d|e|f|g|h|0
	paddw acul, tmp1		; acul = b|c|d|e|f|g|h|0
	movdqu tmp1, srch
	pslldq tmp1, 7*2
	paddw acul, tmp1
	movdqu tmp1, srch
	psrldq tmp1, 1*2
	paddw acuh, tmp1

	lea esi, [esi+edx]		; avanza a la siguiente línea
	movdqu srcl, [esi+ecx]		; srcl = 16px
	movdqu srch, srcl		; srch = srcl
	punpcklbw srcl, tmp3		; srcl = a|b|c|d|e|f|g|h
	punpckhbw srch, tmp3		; srch = i|j|k|l|m|n|o|p
	psubw acul, srcl
	psubw acuh, srch

	sub esi, edx			; vuelvo src a la posición original

%endmacro

section .text
	global asmRoberts

; COSAS A TENER EN CUENTA:
; 	- no hay alineamiento xq es múltiplo de 4 el ancho de la imágen
asmRoberts:
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

	sub dword height, 1
	mov al,xorder			; eax = xorder
	mov ah,yorder			; eax = yorder
	
	pxor tmp3, tmp3

procesar:
	pxor acul,acul			; acul = 0 [acumulador parte baja]
	pxor acuh,acuh			; acuh = 0 [acumulador parte alta]

	RobertsX
	abs acul
	abs acuh

procesar_y:
	RobertsY
	abs acul
	abs acuh

proximos_pixels:
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

