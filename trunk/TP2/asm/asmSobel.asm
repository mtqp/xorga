%include "macros.asm"

section .data
	acum_lineas : DQ 0,0
	acum_filas : DD 0
	test_mask : DD 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	sobel_X_linea1: DW -1,0,1,-1,0,1,-1,0
	sobel_X_linea2: DW -2,0,2,-2,0,2,-2,0
	mask_borrar_px1: DQ 0x00000000ffff0000, 0x000000000000ffff
	mask_borrar_px2: DQ 0x0000ffff00000000, 0x00000000ffff0000
	mask_borrar_px3: DQ 0xffff000000000000, 0x0000ffff00000000
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
;//////////////////////////////////////////////
procesar:
	movdqu xmm0, [test_mask]	; xmm0 = 1...1

ciclo_xorder:

;////////			procesa primer linea
	lea edx, [esi+ecx]
	push edx			; "parámetro" de apply_line (puntero al source)
	push 0				; "parámetro" de apply_line (para q procese con -1 0 1 -1 0 1)
	call apply_lineX		; procesa la línea
	add esp,8			; avanza el tope de la pila
	mov edx,acum_lineas		; copia la parte baja del resultado parcial [instrucción que no hace falta]
	movdqu [edx],xmm7		; acumula linea uno

;	movdqu [edi+ecx], xmm0		; 
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
	sub edx, 14			; edx = ancho - 14
	mov ecx, edx			; columna actual = ancho - 14
	jmp ciclo_xorder		; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	add edi, width			; avanza la fila correspondiente
	add esi, width
	inc ebx				; incremento el contador de filas
	cmp ebx, height			; comparo el contador de filas con la altura
	jne ciclo_xorder		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
apply_lineX:
	pushad				;conviene para poder usar todos los reg
	%define select_line2 	[ebp+12]
	
	mov esi, ptr_src		; esi = puntero al inicio de línea src

	pxor xmm7,xmm7			; xmm7 = 0 [acumulador parte baja]
	pxor xmm6,xmm6			; xmm6 = 0 [acumulador parte alta]

	movdqu xmm0,[esi]		; xmm0 = 16 pixels de src
	movdqu xmm1,xmm0		; duplica xmm0 en xmm1 para futuro uso

	punpcklbw xmm0,xmm7		; xmm0 = primeros 8 pixels pasados a words
	punpckhbw xmm1,xmm7		; xmm1 = segundos 8 pixels pasados a words
				
	;TRABAJO PRIMERO CON PARTE BAJA, PARTE ALTA ANALOGA
	movdqu xmm2,xmm0		; duplico xmm0 en xmm2 temporalmente
	cmp dword select_line2,0x01	; cual de los 2 tipos de linea proceso?
	je line2			; si es -2 0 -2 salta a line2
	movdqu xmm5,[sobel_X_linea1]	; sino, xmm5 = -1 0 -1 ...
	jmp continuar			; salta a "continuar"
line2:
	movdqu xmm5,[sobel_X_linea2]	; xmm5 = -2 0 -2 ...
continuar:
	pmullw xmm0,xmm5		; xmm0 = [primeros 8 pixels] * [línea de la matríz]
					; como las matrices tienen 0s quedaría:
					; xmm0 = [a,0,c,d,0,f,g,0]
	pshufd xmm3,xmm0,00001001b	; xmm3 = [c,d,-,-,-,-,-,-]
	pshuflw xmm3,xmm3,01000010b
				;xmm3 == [C,fruta,fruta,F,fruta,fruta,gdaf,graf]
				;xmm0 == [A,frefr,frefr,D,frefr,frefr,fref,fref]
	paddw xmm0,xmm3		;xmm0 == [B,fruta,fruit,E,fruit,fruit,fruit,fru]
	pslldq xmm0,2		;xmm0 == [frut,B,fruit,fruto,E,fruti,frtu,friut]
	movdqu xmm4,[mask_borrar_px1]
	andps xmm0,xmm4		;xmm0 == [0,B,0,0,E,0,0,0]
	paddw xmm7,xmm0			;xmm7 == xmm0
					
	movdqu xmm0,xmm2
	pslldq xmm5,2			;xmm5 == [0,-1,0,1,-1,0,1,-1]
	pmullw xmm0,xmm5		;[o,b,C,d,e,F,g,o]
					;C<-b+c+d,F<-e+f+g	(c y f == 0)
	pshuflw xmm3,xmm0,00001100b	;xmm3 == [-,d,-,-,e,f,g,-]
	pshufhw xmm3,xmm3,00000010b	;xmm3 == [-,d,-,-,g,-,-,-]
					;xmm0 == [-,b,-,-,e,-,-,-]
	paddw xmm0,xmm3			;xmm0 == [-,C,-,-,F,-,-,-]
	pslldq xmm0,2			;xmm0 == [-,-,C,-,-,F,-,-]
	movdqu xmm4,[mask_borrar_px2]
	andps xmm0,xmm4			;xmm0 == [0,0,C,0,0,F,0,0]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,0,e,f,0,0]
					
	movdqu xmm0,xmm2
	pslldq xmm5,2			;xmm5 ==  [0,0,-1,0,1,-1,0,1]
	pmullw xmm0,xmm5		;xmm0 == [-,-,c,D,e,f,G,h]
	
	pshufd xmm3,xmm0,00111000b	;xmm3 == [-,-,e,f,g,h,-,-]
	paddw xmm0,xmm3			;xmm0 == [-,-,D,-,-,G,-,-]
	pslldq xmm0,2			;xmm0 == [-,-,-,D,-,-,G,-]
	movdqu xmm4,[mask_borrar_px3]
	andps xmm0,xmm4
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,0]
					
				;falta ver mas signific de la parte baja
				;y el menos sig de la parte alta
bordes_xmm:			;en xmm1 estan las ocho words altas
	movdqu xmm0,xmm2
	movdqu xmm4,[mask_2words_masSig]
	andps xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,g,h]
	movdqu xmm3,xmm1		;xmm3 == parteALTA
	movdqu xmm4,[mask_2words_menSig]
	andps xmm3,xmm4			;xmm3 == [aH,bH,0,0,0,0,0,0]
	paddw xmm0,xmm3			;xmm0 == [aH,bH,0,0,0,0,g,h]
	pshufd xmm0,xmm0,00111100b	;xmm0 == [aH,bH,g,h,g,h,aH,bH]
	pshuflw xmm3,xmm0,00001100b	;xmm3 == [++,-h,+,+,+,+,-g,++]
	pshufhw xmm3,xmm3,00000000b
	movdqu xmm4,[neg_1]
	pmullw xmm3,xmm4
	
	paddw xmm0,xmm3			;xmm0 == [- ,AH,-,-,--,-,HH, -]
	pshuflw xmm0,xmm0,00000001b
	pshufhw xmm0,xmm0,10000000b
	movdqu xmm4,[mask_primYult]
	andps xmm0,xmm4			;xmm0 == [AH,-,-,-,-,-,-,-,HH]
	movdqu xmm6,xmm0
	movdqu xmm4,[mask_2words_menSig]
	andps xmm6,xmm4			;xmm6 == [aH,0,0,0,0,0,0,0]
	movdqu xmm4,[mask_2words_masSig]
	andps xmm0,xmm4			;xmm0 == [0,0,0,0,0,0,0,h]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,h]
	

parteAlta:
	movdqu xmm2,xmm1		;salvo x un momento la matriz
	cmp dword select_line2,0x01
	je line2H
	movdqu xmm5,[sobel_X_linea1]
	jmp continuarH
line2H:
	movdqu xmm5,[sobel_X_linea2]
continuarH:
	pmullw xmm1,xmm5		;[a,B,c,d,E,f,o,o]
					;b<-a+b+c,E<-d+e+f (b y e son ceros!)
	pshufd xmm3,xmm1,00000110b	;? [E,F,C,D,Z,Z,Z,Z]
	pshuflw xmm3,xmm3,01000010b
				;xmm3 == [C,fruta,fruta,F,fruta,fruta,gdaf,graf]
				;xmm0 == [A,frefr,frefr,D,frefr,frefr,fref,fref]
	paddw xmm1,xmm3		;xmm0 == [B,fruta,fruit,E,fruit,fruit,fruit,fru]
	pslldq xmm1,2		;xmm0 == [frut,B,fruit,fruto,E,fruti,frtu,friut]
	movdqu xmm4,[mask_borrar_px1]
	andps xmm1,xmm4		;xmm0 == [0,B,0,0,E,0,0,0]
	paddw xmm6,xmm1			;xmm7 == xmm0
					
	movdqu xmm1,xmm2
	pslldq xmm5,2			;xmm5 == [0,-1,0,1,-1,0,1,-1]
	pmullw xmm1,xmm5		;[o,b,C,d,e,F,g,o]
					;C<-b+c+d,F<-e+f+g	(c y f == 0)
	pshuflw xmm3,xmm1,00001100b	;xmm3 == [-,d,-,-,e,f,g,-]
	pshufhw xmm3,xmm3,00000010b	;xmm3 == [-,d,-,-,g,-,-,-]
					;xmm0 == [-,b,-,-,e,-,-,-]
	paddw xmm1,xmm3			;xmm0 == [-,C,-,-,F,-,-,-]
	pslldq xmm1,2			;xmm0 == [-,-,C,-,-,F,-,-]
	movdqu xmm4,[mask_borrar_px2]
	andps xmm1,xmm4			;xmm0 == [0,0,C,0,0,F,0,0]
	paddw xmm6,xmm1			;xmm7 == [0,b,c,0,e,f,0,0]
					
	movdqu xmm1,xmm2
	pslldq xmm5,2			;xmm5 ==  [0,0,-1,0,1,-1,0,1]
	pmullw xmm1,xmm5		;xmm0 == [-,-,c,D,e,f,G,h]
	
	pshufd xmm3,xmm1,00111000b	;xmm3 == [-,-,e,f,g,h,-,-]
	paddw xmm1,xmm3			;xmm0 == [-,-,D,-,-,G,-,-]
	pslldq xmm1,2			;xmm0 == [-,-,-,D,-,-,G,-]
	movdqu xmm4,[mask_borrar_px3]
	andps xmm1,xmm4
	paddw xmm6,xmm1			;xmm7 == [0,b,c,d,e,f,g,0]
					

fin_apply_line:

	packuswb xmm7,xmm6
	;movdqu [edi],xmm7
	;convencion_C_fin
	popad
	ret

