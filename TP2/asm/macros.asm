%macro convencion_C_mem 1
	push ebp
	mov ebp,esp
	sub esp,%1
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_C_fin_mem 1
	pop ebx
	pop esi
	pop edi
	add esp,%1
	pop ebp
	ret
%endmacro

%macro convencion_C 0
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_C_fin 0
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
%endmacro

%macro getLineSize 2		; getLineSize(width, align)
	mov eax, %1		;eax = width
	mov esi, %2		;esi = 4
	xor edx, edx		;edx = 0		
	div esi			;eax = width/align , edx = width%align

	mov eax, %1		;eax = width	
	cmp edx, 0		;el ancho era múltiplo de 4?
	je  .return		;si ya era, entonces devuelve el line

	sub eax, edx		;eax = width - width%align
	add eax, esi		;eax = width - witdh%align + align
.return:
	mov line, eax
%endmacro

; procesarFragmentoX input, acumulador, shuf1, shuf2, máscara_px_procesados
; 	aplica la línea de la matríz para 2px del input y los suma en el
;	acumulador. Los px procesados son seleccionados mediante las máscaras.
%macro procesarFragmentoX 3
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

