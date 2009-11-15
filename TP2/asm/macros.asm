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

; abs xmm
;	calcula el valor absoluto del registro (packed words)
%macro abs 1
	pxor xmm3,xmm3
	pcmpgtw xmm3,%1
	pxor %1,xmm3
	psubw %1,xmm3
%endmacro

%macro ProcesarFreiY 0
				cvtdq2ps  xmm3,xmm3  ;Convierto los dwords en floats
				movdqu xmm2,xmm3  ;copio el xmm3 que tiene los datos en floats
				mulps  xmm2,xmm7  ;multiplico por raiz de 2 los valores del medio
				
				;tengo xmm2 = 1 2* 3* 4 
				haddps xmm2,xmm2 			;ahora los tengo sumados de a pares: 1+2* | 3*+4 |
				shufps xmm3,xmm3,01100110b  ;shufleo el xmm3 para q en los primeros lugares tenga el 3 y 2 para completar la suma
				addps xmm2,xmm3  			;tengo en el primer y segundo lugar de xmm2 dos pixeles ya procesados				
				cvtps2dq xmm2,xmm2          ;convierto todo a ints
				abs xmm2  					;la macro usa el xmm3 q ya no lo necesito
				packssdw xmm2,xmm2
				packuswb xmm2,xmm2
				movd 	 eax,xmm2
				mov word [edi],ax			
				add edi,2
%endmacro



;;;///*** FreiChen en Y ***///;;;
%macro FreiChenY 0

				;;Idea: Con restar la linea superior e inferior y multiplicar adecuadamente por raiz de 2, ya voy a tener los pixeles que quiero ;;





				;cargo linea de arriba y abajo, ebx es el width y edx el offset

				mov eax,esi	 					;uso esta direccion temporal porque no tengo modo de direccionamiento para lo q quiero
				lea eax,[eax+edx]				;sumo offset
				movdqu xmm1, [eax]	
				movdqu xmm0, [eax+2*ebx]	

				;las resto, xmm0 = xmm0-xmm1 (linea inferior - superior) (unsigned)
				psubusb xmm0,xmm1
			

				;hago una mascara para saber q bytes son negativos 
				pxor xmm2,xmm2
				pcmpgtb xmm2, xmm0				;si un byte de xmm0 era negativo, en xmm2 hay 0xFF, sino 0x00


				;desenpaqueto la parte baja y alta, extendiendo el signo con la mascara del paso anterior
				movdqu xmm1,xmm0				;copio temporalmente el xmm0, en xmm1 que ya no lo necesito
				punpckhbw xmm0,xmm2 			;xmm0 = parte alta		(puedo sobreescribir xmm0 porq no lo voy a necesitar mas)
				punpcklbw xmm1,xmm2 			;xmm1 = parte baja




				;;Tengo, en este punto, en xmm0 y xmm1 dos sets de 8 pixeles a los que llamo: 
					;xmm0 = 12345678
					;xmm1 = abcdefgh
				 ;Para laburar en punto flotante voy a tener que agarrar de a 4 datos (32b c/u)
		


				pcmpgtw xmm4,xmm1   			;voy creando la nueva mascara para abcdefgh
				movdqu  xmm3,xmm1 
				punpcklwd xmm3,xmm4 			;Ahora con efgh
				ProcesarFreiY

				movdqu  xmm3,xmm1 
				movdqu  xmm5,xmm1   			;tambien lo copio aca temporalmente
				punpckhwd xmm5,xmm4 			;Agarro otra vez el abcd
				punpcklwd xmm3,xmm4 			;Ahora con efgh
				shufps  xmm3,xmm5,00011011b 	;shuffleo y tengo cdef
				ProcesarFreiY	

				movdqu  xmm3,xmm1
				punpckhwd xmm3,xmm4 			;tengo abcd
				ProcesarFreiY


				movdqu  xmm3,xmm0				;agarro 12345678 
				movdqu  xmm5,xmm1				;agarro abcdefgh
				punpckhwd xmm5,xmm4 			;agarro abcd
				pcmpgtw xmm4,xmm0   			;voy creando la nueva mascara para abcdefgh
				punpcklwd xmm3,xmm4 			;agarro 5678
				shufps xmm3,xmm5,11100100b 		;shuffleo y tengo 78ab
				shufps xmm3,xmm3,00011011b		;Doy vuelta el registro con otro shuffle para facilitarme la vida..
				ProcesarFreiY



				movdqu  xmm3,xmm0 
				punpcklwd xmm3,xmm4 			;Ahora con 5678
				ProcesarFreiY	
		

				movdqu  xmm3,xmm0 
				movdqu  xmm5,xmm0   			;tambien lo copio aca temporalmente
				punpckhwd xmm5,xmm4 			;Agarro otra vez el 1234
				punpcklwd xmm3,xmm4 			;Ahora con 5678
				shufps  xmm3,xmm5,00011011b 	;shuffleo y tengo 3456 10110001b
				ProcesarFreiY			
				


				;Proceso el 1234 de xmm0, y tengo en xmm3 el 1234 en floats
				pxor    xmm4,xmm4
				pcmpgtw xmm4,xmm0 				;desde este punto en xmm4 tengo la mascara para la parte baja (12345678)
				movdqu  xmm3,xmm0 				;(copio xmm0 temporalmente)
				punpckhwd xmm3,xmm4  			;mezclo parte alta de xmm3 (1234) con la mascara q me da la extension del signo
				ProcesarFreiY							;requiere el xmm2 libre, el xmm7 con las raices de 2, y el xmm3 con la parte a procesar y guardar

%endmacro
