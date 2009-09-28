extern apply_mask

%macro convencion_C 1
	push ebp
	mov ebp,esp
	sub esp,%1
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_C_fin 1
	pop ebx
	pop esi
	pop edi
	add esp,%1
	pop ebp
%endmacro

%macro adjustLineEnebx 0
			;ajusto linea
			;l(w,a) = w - (w%a) + a(w%a>0)
	mov eax,width		;muevo el ancho a eax
	mov esi, align		;el alineamiento a 4 q quiero
	xor edx,edx
	div esi			;eax = quotient(width/align)
				;edx = resto(width/align)

				;tengo q saber si tengo resto o no
				;o sea cuantos pixels basura hay
	mov ebx, width
	sub ebx, edx            ; w - w%a

	cmp edx, 0
	je procesar		;si salta aca es xq NO hay basura

	add ebx, align		;tengo en ebx = w- (w%a) + a(w%a>0) == line
	mov dword line,ebx

	mov eax, width
	mov dword fill,ebx
	sub dword fill,eax	;guardo la cant de pixels basura

%endmacro

section .data

	sobelX: db -1,0,1,-2,0,2,-1,0,1
	sobelY: db -1,-2,-1,0,0,0,1,2,1

section .text

global asmSobel

asmSobel:
	convencion_C 8

	%define line [ebp-4]
	%define fill [ebp-8]
	%define align 4
	%define mask_size 3
	%define ptr_imgSrc [ebp+8]
	%define ptr_imgDst [ebp+12]
	%define width [ebp+16]
	%define height	 [ebp+20]
	%define x_order	 [ebp+24]
	%define y_order	 [ebp+28]

	adjustLineEnebx

procesar:
	dec dword width		;porq necesito el width -1
	dec dword height	;xq necesito el height-1
	mov ecx,1		;contador columna
	mov esi,1		;contador fila

	xor edx,edx		;para manejar offset

cicloFilas:

	cicloColumnas:

		lea edx,[edx+ecx-1]	;edx = ptr_src
		xor edi,edi	;registro acumulador del pixel
		push ecx	;salvo contador columnas
				;salvo acumulador para llamar a apply_mask
	X:
		push mask_size
		push sobelX	;[ebp-20] esta el sobel X... creemos
		push dword line	;antes estaba puesto en ebp-16 SOBELX
		push edx	;prt_src
		cmp dword x_order,0
		je Y
		call apply_mask
		add edi,eax	;acumulo el resultado del apply_mask
	Y:
		cmp dword y_order,0
		je continuar
		mov dword [ebp-20],sobelY	;cambia sobelY
		call apply_mask
		add edi,eax

	continuar:

		add esp,16	;reapunto el esp
				;en ebx tengo el line
		pop ecx
		mov dword ptr_imgDst,edi	;guardo la imagen
		inc dword ptr_imgDst
		inc ecx
		cmp ecx,width
		jne cicloColumnas

	mov eax, fill
	add dword ptr_imgDst,eax
	mov edx,ebx
	add ebx,line
	inc esi
	cmp esi,height
	jne cicloFilas

	convencion_C_fin 8
