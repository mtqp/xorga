%include "macros.asm"
extern apply_lineFC

;////aridad        void apply_lineFC(char* ptr_src,int select_line);
;						POR EL MOMENTO NO LO GUARDO EN MEM, SINO QDAN EN XMM0----XMM3
section .data
	acum_res_bajo: DQ 0,0,0,0
	acum_res_alto: DQ 0,0,0,0
	
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
	%define line_1		0
	%define line_2		1
	%define line_3		0

	xor ecx, ecx			; ecx = 0 [contador de columnas]
	xor ebx, ebx			; ebx = 0 [contador de filas]
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	mov edi, ptr_dst		; edi = puntero al inicio de línea dst
	add edi,width			; avanza edi una línea xq la 1er línea va en negro
;	dec height				; la ultima linea NO se procesa

;////////////////////////////////////////////////////////////////
procesar_FC:
	lea edx,[esi+ecx]
	push edx
	push line_1
	call apply_lineFC
	add esp,8
	movdqu [acum_res_bajo],xmm0
	movdqu [acum_res_bajo+16],xmm1
	movdqu [acum_res_alto],xmm2
	movdqu [acum_res_alto+16],xmm3

	;lea edx,[esi+ecx]
	add edx,width
	push edx
	push line_2
	call apply_lineFC
	add esp,8
	movdqu xmm4,[acum_res_bajo]
	addps xmm0,xmm4
	movdqu [acum_res_bajo],xmm0
	
	movdqu xmm4,[acum_res_bajo+16]
	addps xmm1,xmm4
	movdqu [acum_res_bajo+16],xmm1
	
	movdqu xmm4,[acum_res_alto]
	addps xmm2,xmm4
	movdqu [acum_res_alto],xmm2
	
	movdqu xmm4,[acum_res_alto+16]
	addps xmm3,xmm4
	movdqu [acum_res_alto+16],xmm3

	;lea edx,[esi+ecx]
	add edx,width
	push edx
	push line_3
	call apply_lineFC
	add esp,8
	
	movdqu xmm4,[acum_res_bajo]
	addps xmm0,xmm4
	
	movdqu xmm4,[acum_res_bajo+16]
	addps xmm1,xmm4
	
	movdqu xmm4,[acum_res_alto]
	addps xmm2,xmm4
	
	movdqu xmm4,[acum_res_alto+16]
	addps xmm3,xmm4
	
										;tengo q armar los 16 bytes saturados ahora y mandarlos a mem!
	;CVTPS2DQ------>los paso a words integers
	cvtps2dq xmm0,xmm0
	cvtps2dq xmm1,xmm1
	cvtps2dq xmm2,xmm2
	cvtps2dq xmm3,xmm3
	
	packssdw xmm0,xmm1					;paso los integers a words
	packssdw xmm2,xmm3
	
	packuswb xmm0,xmm2					;paso a byte saturado
	
	movdqu [edi+ecx],xmm0					;guardo en mem
;//////////////////////////////////////////
	add ecx,14			; avanza 14 columnas
					; verifica si está cerca del final de la línea
	mov edx, ecx			; edx = columna actual (temporalmente)
	add edx, 14			; edx = columna actual + 14
	cmp edx, width			; verifica si al procesar 14 bytes se pasa o no del ancho
	jl procesar_FC			; si no se pasa continúa procesando
					; si se pasa..
	cmp ecx,width			; compara la columna actual con el ancho de la imágen
	je avanzar_linea		; si son iguales avanza a la siguiente fila
					; sino reposiciona la columna para terminar de procesar
	mov edx, width			; edx = ancho
	sub edx, 14			; edx = ancho - 14 [posible segfault! lo dejo porque anda por ahora]
	mov ecx, edx			; columna actual = ancho - 14
	jmp procesar_FC			; procesa el último ciclo de la fila

avanzar_linea:
	xor ecx, ecx			; resetea el contador de columnas
	add edi, width			; avanza la fila correspondiente
	add esi, width
	inc ebx				; incrementa el contador de filas
	cmp ebx, height			; compara el contador de filas con la altura
	jne procesar_FC	 		; mientras no llegue a la última línea, sigo procesando
	convencion_C_fin_mem 16


;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

