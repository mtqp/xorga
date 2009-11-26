;///aridad funcion      void apply_lineFC(char * ptr_src,char* ptr_dst,int witdh_line)

%include "macros.asm"

;matriz freichen X
;-1	 	0	 1
;-r(2)	0	r(2)
;-1		0	 1

;matriz roberts Y
;-1		-r(2)	-1
; 0		 0		 0
; 1		 r(2)	 1

;///////////la idea es procesar de a dos px en X y de a uno en Y
section .data
	acum_res_bajo: DQ 0,0,0,0
	acum_res_alto: DQ 0,0,0,0
	doubles_del_medio: DD 0,-1,-1,0
	negar_freichen_X: DD -1.0,-1.0,1.0,1.0
	negar_freichen_Y: DD -1.0,-1.0,-1.0,-1.0
	freichen_X_linea1: DD -1.0,  -1.0, 1.0, 1.0
	freichen_X_linea2: DD  2.0,  2.0, 2.0, 2.0		;empieza con la raiz calculada
	freichen_Y_linea1: DD  1.0, 2.0, 1.0, 0		;este empieza el ciclo negado y con la raiz calculada
	freichen_Y_linea3: DD  1.0, 2.0, 1.0, 0		;empieza con la raiz calculada
	mask_limpiar: 	 DQ 0xFFFF00000000FFFF, 0x0000000000000000
	mask_2words_masSig: DQ 0, 0xffffffff00000000
	mask_2words_menSig: DQ 0x00000000ffffffff, 0
section .text
	global apply_lineFC

apply_lineFC:
	pushad					;---->ojo q siempre trae franco esto
	;convencion_C
	%define ptr_src		[ebp+8]
	;%define ptr_dst		[ebp+12]
	;%define width		[ebp+16]
	%define line_selector	[ebp+12]		;si el selector esta en cero hago -101 si esta en 1,hago -sqr2 0 sqr2
	;%define xorder		[ebp+24]
	;%define yorder 		[ebp+28]

;/////ARMO LAS RAICES Y LOS NUMEROS EN NEGATIVO//////////////
	movdqu xmm0,[freichen_X_linea2]
	sqrtps xmm0,xmm0		;guardo ya con las raices!
	movdqu xmm1,[negar_freichen_X]
	mulps xmm0,xmm1
	movdqu [freichen_X_linea2],xmm0			;[freichen_X_linea2] = [-sqr(2),-sqr(2),sqr(2),sqr(2)]
	movdqu xmm0,[freichen_Y_linea1]
	sqrtps xmm0,xmm0
	movdqu xmm1,xmm0
	movdqu xmm2,[negar_freichen_Y]
	mulps xmm0,xmm2
	movdqu [freichen_Y_linea1],xmm0			; = [-1,-sqr(2),-1,-0]
	movdqu [freichen_Y_linea3],xmm1			; = [1,sqr(2),1,0]
;////////////////////////////////////////////////////////////
	mov esi, ptr_src		; esi = puntero al inicio de línea src
	;mov edi, ptr_dst		; edi = puntero al inicio de línea dst
	
	mov ebx,line_selector	; ebx = calculo la linea 1 o la 2???
	pxor xmm7,xmm7			; xmm7 = acum de lo mas bajo de lo mas bajo?
fcX:
;//////////////////LINEA -101////////////////////////////////
	movdqu xmm0,[esi]		;tengo la primer linea
	punpcklbw xmm0,xmm7
	movdqu xmm1,xmm0
	punpcklwd xmm0,xmm7		;xmm0 = [a,b,c,d]
	punpckhwd xmm1,xmm7		;xmm1 = [e,f,g,h]
	cvtdq2ps xmm0,xmm0		; (float) xmm0
	cvtdq2ps xmm1,xmm1

	movdqu xmm4,xmm0		;salvo [a,...,h]
	movdqu xmm5,xmm1
	
	cmp ebx,0
	jne pb_raiz
	movdqu xmm3,[negar_freichen_X]
	jmp comenzar_pb
pb_raiz:
	movdqu xmm3,[freichen_X_linea2]
comenzar_pb:
	
	mulps xmm0,xmm3			;B=a+c, C=b+d
	mulps xmm1,xmm3			;F=e+g, G=f+h
							;[0,B,C,0,0,F,G,0]	->QUIERO LLEGAR A ESO
	movdqu xmm3,xmm0		;xmm3 = [-a,-b,c,d]
	psrldq xmm3,8			;xmm3 = [c,d,0,0]
	addps xmm0,xmm3			;xmm0 = [B,C,-,-]
	pslldq xmm0,8			;xmm0 = [0,0,B,C]	
	psrldq xmm0,4			;xmm0 = [0,B,C,0]
	movdqu xmm7,xmm0		;xmm7 = acum<<
	
	movdqu xmm3,xmm1		;xmm3 = [-e,-f,g,h]
	psrldq xmm3,8			;xmm3 = [g,h,0,0]
	addps xmm1,xmm3			;xmm1 = [F,G,-,-]
	pslldq xmm1,8			;xmm1 = [0,0,F,G]
	psrldq xmm1,4			;xmm1 = [0,F,G,0]
	movdqu xmm6,xmm1		;xmm6 = acum<
	
							;necesito tener reg = [-c,-d,e,f]
	psrldq xmm4,8			;xmm4 = [c,d,0,0]
	pslldq xmm5,8			;xmm5 = [0,0,e,f]
	addps xmm4,xmm5			;xmm4 = [c,d,e,f]
	
	cmp ebx,0
	jne pb_raiz2
	movdqu xmm3,[negar_freichen_X]
	jmp comenzar_pb2
pb_raiz2:
	movdqu xmm3,[freichen_X_linea2]
comenzar_pb2:
	
	mulps xmm4,xmm3			;xmm4 = [-c,-d,e,f]
	movdqu xmm5,xmm4
	psrldq xmm5,8			;xmm5 = [e,f,0,0]
	addps xmm4,xmm5			;xmm4 = [D,E,-,-]
	pslldq xmm4,8			;xmm4 = [0,0,D,E]
	movdqu xmm5,xmm4
	pslldq xmm4,4			;xmm4 = [0,0,0,D]
	psrldq xmm5,12			;xmm5 = [E,0,0,0]
	addps xmm7,xmm4
	addps xmm6,xmm5			;[0,B,C,D,E,F,G,0]
	
	movdqu [acum_res_bajo],xmm7
	movdqu [acum_res_bajo+16],xmm6
;////////////////////////////////////////////////////////////
;-----------------------SEGUNDOS 8 BYTES A PROCESAR----------
;////////////////////////////////////////////////////////////
	pxor xmm7,xmm7
	movdqu xmm0,[esi]		;tengo la primer linea
	punpckhbw xmm0,xmm7
	movdqu xmm1,xmm0
	punpcklwd xmm0,xmm7		;xmm0 = [a,b,c,d]
	punpckhwd xmm1,xmm7		;xmm1 = [e,f,g,h]
	cvtdq2ps xmm0,xmm0		; (float) xmm0
	cvtdq2ps xmm1,xmm1
	
	movdqu xmm4,xmm0		;salvo [a,...,h]
	movdqu xmm5,xmm1
	
	cmp ebx,0
	jne pb_raiz3
	movdqu xmm3,[negar_freichen_X]
	jmp comenzar_pa
pb_raiz3:
	movdqu xmm3,[freichen_X_linea2]
comenzar_pa:
	
	mulps xmm0,xmm3			;B=a+c, C=b+d
	mulps xmm1,xmm3			;F=e+g, G=f+h
							;[0,B,C,0,0,F,G,0]	->QUIERO LLEGAR A ESO
	movdqu xmm3,xmm0		;xmm3 = [-a,-b,c,d]
	psrldq xmm3,8			;xmm3 = [c,d,0,0]
	addps xmm0,xmm3			;xmm0 = [B,C,-,-]
	pslldq xmm0,8			;xmm0 = [0,0,B,C]	
	psrldq xmm0,4			;xmm0 = [0,B,C,0]
	movdqu xmm7,xmm0		;xmm7 = acum<<
	
	movdqu xmm3,xmm1		;xmm3 = [-e,-f,g,h]
	psrldq xmm3,8			;xmm3 = [g,h,0,0]
	addps xmm1,xmm3			;xmm1 = [F,G,-,-]
	pslldq xmm1,8			;xmm1 = [0,0,F,G]
	psrldq xmm1,4			;xmm1 = [0,F,G,0]
	movdqu xmm6,xmm1		;xmm6 = acum<
	
							;necesito tener reg = [-c,-d,e,f]
	psrldq xmm4,8			;xmm4 = [c,d,0,0]
	pslldq xmm5,8			;xmm5 = [0,0,e,f]
	addps xmm4,xmm5			;xmm4 = [c,d,e,f]



cmp ebx,0
	jne pb_raiz4
	movdqu xmm3,[negar_freichen_X]
	jmp comenzar_pa2
pb_raiz4:
	movdqu xmm3,[freichen_X_linea2]
comenzar_pa2:
	
	mulps xmm4,xmm3			;xmm4 = [-c,-d,e,f]
	movdqu xmm5,xmm4
	psrldq xmm5,8			;xmm5 = [e,f,0,0]
	addps xmm4,xmm5			;xmm4 = [D,E,-,-]
	pslldq xmm4,8			;xmm4 = [0,0,D,E]
	movdqu xmm5,xmm4
	pslldq xmm4,4			;xmm4 = [0,0,0,D]
	psrldq xmm5,12			;xmm5 = [E,0,0,0]
	addps xmm7,xmm4
	addps xmm6,xmm5			;[0,B,C,D,E,F,G,0]

	movdqu [acum_res_alto],xmm7
	movdqu [acum_res_alto+16],xmm6

;//////////////////////////////////////////////////////////
;----------bytes intermedios entre baja y alta-------------
							;me falta HL y AH
							;HL == -g + ah
							;AH == -h + bh
	pxor xmm7,xmm7
	movdqu xmm0,[esi]		;tengo la primer linea
	movdqu xmm1,xmm0
	punpcklbw xmm1,xmm7
	punpckhwd xmm1,xmm7		;xmm1 = [e,f,g,h]
	punpckhbw xmm0,xmm7
	punpcklwd xmm0,xmm7		;xmm0 = [aH,bH,cH,dH]
	
	cvtdq2ps xmm0,xmm0		; (float) xmm0
	cvtdq2ps xmm1,xmm1

	movdqu xmm4,xmm0		;salvo [a,...,h]
	movdqu xmm5,xmm1
	
	psrldq xmm1,8			;xmm1 = [g,h,0,0]
	pslldq xmm0,8			;xmm0 = [0,0,ah,bh]
	addps xmm0,xmm1			;xmm0 = [g,h,ah,bh]
	
	cmp ebx,0
	jne pb_raizMEDIO
	movdqu xmm3,[negar_freichen_X]
	jmp comenzar_medio
pb_raizMEDIO:
	movdqu xmm3,[freichen_X_linea2]
comenzar_medio:
	
	mulps xmm0,xmm3			;xmm0 = [-g,-h,ah,bh]
	
							;[aH,0,0,0,0,0,0,H]	->QUIERO LLEGAR A ESO
	movdqu xmm3,xmm0		;xmm3 = [-g,-h,ah,bh]
	psrldq xmm3,8			;xmm3 = [ah,bh,0,0]
	addps xmm0,xmm3			;xmm0 = [H,AH,-,-]
	pslldq xmm0,8			;xmm0 = [0,0,H,AH]	
	movdqu xmm1,xmm0		;salvo el reg x un sec
	
	pslldq xmm0,4			;xmm0 = [0,0,0,H]
	psrldq xmm1,12			;xmm1 = [aH,0,0,0]
	
	movdqu xmm6,[acum_res_bajo+16]
	addps xmm6,xmm0			;xmm6 = [E,F,G,H]
	
	movdqu xmm7,[acum_res_alto]
	addps xmm7,xmm1			;xmm1 = [AH,BH,CH,DH]
	movdqu [acum_res_bajo+16],xmm6
	movdqu [acum_res_alto],xmm7

save_mem:
	movdqu xmm0,[acum_res_bajo]
	movdqu xmm1,[acum_res_bajo+16]
	movdqu xmm2,[acum_res_alto]
	movdqu xmm3,[acum_res_alto+16]
	
	;movdqu [edi],xmm0
	;add edi,16
	;movdqu [edi],xmm1
	;add edi,16
	;movdqu [edi],xmm2
	;add edi,16
	;movdqu [edi],xmm3
;/////////////////LINEA -202/////////////////////////////////


;////////////////////////////////////////////////////////////
;fcY:

;///////////////////////////////////////////////////////////
	;convencion_C_fin
	popad			;--->ojo q siepmre trae franco esto
