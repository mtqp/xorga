
%include "raices.asm"

%macro frei 0

	movdqu xmm0, [esi]   ; xmm0 y xmm1 (bytes) = fila arriba
	movdqu xmm1, [esi+edx]
	movdqu xmm2, [esi+edx*2]   ; xmm4 y xmm5 (bytes) = fila de abajo

	;--------------+
	;  Frei en X   ;
	;--------------+

		pxor xmm7,xmm7

		movss xmm6, AUX
		pshufd xmm6, xmm6, 00000000b

		raices xmm1, xmm3, xmm4, xmm5

	; desempaqueto xmm0

	movdqu xmm5,xmm0
	movdqu xmm6,xmm0
	punpcklbw xmm5,xmm7		; xmm3 = low
	punpckhbw xmm6,xmm7		; xmm4 = hi

	; sumo xmm1 += xmm0

	paddw xmm3, xmm5
	paddw xmm4, xmm6

	; desempaqueto xmm2

	movdqu xmm5,xmm2
	movdqu xmm6,xmm2
	punpcklbw xmm5,xmm7		; xmm3 = low
	punpckhbw xmm6,xmm7		; xmm4 = hi

	; sumo xmm1 += xmm2

	paddw xmm3, xmm5
	paddw xmm4, xmm6

	; duplico xmm5 = xmm3 y xmm6 = xmm4

	movdqu xmm5, xmm3
	movdqu xmm6, xmm4

	; hago magia para shiftear
	
	movdqu xmm7, xmm6
	pslldq xmm7, 12

	psrldq xmm5, 4
	paddw xmm5, xmm7

	psrldq xmm6, 4

	psubw xmm5, xmm3
	psubw xmm6, xmm4

	; tomo valor absoluto y saturo

	pxor xmm7,xmm7
	pcmpgtw xmm7,xmm5
	pxor xmm5,xmm7
	psubw xmm5,xmm7
	pxor xmm7,xmm7
	pcmpgtw xmm7,xmm6
	pxor xmm6,xmm7
	psubw xmm6,xmm7

	; Empaqueto el resultado

	psraw xmm5, 1
	psraw xmm6, 1

	packuswb xmm5,xmm6

	;--------------+
	;  Frei en Y   ;
	;--------------+

	psubb xmm2,xmm0			; xmm2 (bytes) = fila abajo - fila arriba
	movdqu xmm0, xmm5		; guardo en xmm0 el resultado de prewittX
	movdqu xmm4,xmm2

	pxor xmm7,xmm7
	pcmpgtb xmm7, xmm2

	; Desempaqueto filaAbajo-filaArriba

	movdqu xmm3,xmm2
	punpcklbw xmm2,xmm7		; xmm2 = low
	punpckhbw xmm3,xmm7		; xmm3 = hi

	; Desempaqueto filaAbajo-filaArriba << 1

	psrldq xmm4,1           ; xmm4 = filaAbajo-filaArriba << 1

	movss xmm6, AUX
	pshufd xmm6, xmm6, 00000000b

	raices2 xmm4, xmm5, xmm6, xmm1 ; xmm5 = low, xmm6 = high

	; Sumo a cada numero, el de la derecha

	paddw xmm2,xmm5
	paddw xmm3,xmm6
			
	; Desempaqueto filaAbajo-filaArriba << 2
			
	psrldq xmm4,1				; xmm4 = filaAbajo-filaArriba << 2

	pxor xmm7,xmm7
	pcmpgtb xmm7, xmm4

	movdqu xmm5,xmm4
	movdqu xmm6,xmm4
	punpcklbw xmm5,xmm7		; xmm5 = low
	punpckhbw xmm6,xmm7     ; xmm6 = hi
			
	; Sumo a cada numero, el de dos mas a la derecha

	paddw xmm2,xmm5			; xmm2 = resultado low
	paddw xmm3,xmm6         ; xmm3 = resultado hi

	; tomo valor absoluto y saturo

	pxor xmm7,xmm7
	pcmpgtw xmm7,xmm2
	pxor xmm2,xmm7
	psubw xmm2,xmm7
	pxor xmm7,xmm7
	pcmpgtw xmm7,xmm3
	pxor xmm3,xmm7
	psubw xmm3,xmm7
			
	; Empaqueto el resultado

	psraw xmm2, 1
	psraw xmm3, 1

	packuswb xmm2,xmm3

	; Sumo freiX y freiY

	paddusb xmm0,xmm2
	movdqu [edi],xmm0

	lea esi,[esi+14]
	lea edi,[edi+14]

%endmacro

asmFrei:
	Entrar
	init

	finit
	fld1
	fld1
	faddp
	fsqrt
	fstp dword AUX

	convolucion frei

	Salir

