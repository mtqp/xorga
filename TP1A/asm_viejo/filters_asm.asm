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
%endmacro

section .text
	global  apply_mask

apply_mask:
	convencion_C

	%define imgSrc [ebp+8]
	%define line [ebp+12]
	%define ptr_mask [ebp+16]
	%define mask_size [ebp+20]

	mov esi,imgSrc	;tengo la posicion del primer pixel!
	xor ebx,ebx	;contador que va de 0 a mask_size pH y pL
	xor edi,edi	;resultado parcial!
	mov ecx,ptr_mask
cicloFilas:

	cicloColumnas:
		movsx dx,[ecx]	;extension de signo de mascara SE PUEDE USAR?
		;mov al,[ecx]	;extension de signo de mascara
		;cbw
		;mov edx,eax
		xor eax,eax
		mov al, [esi]	;guardo el primer pixel
		imul dx		;multiplico x mascara
		cwde
		add  edi, eax	;resultado parcial incrementado
		inc esi		;me corro de pixel
		inc ecx		;me corro en la mascara

		inc bl		;parte baja es columna
		cmp bl,byte mask_size
		jne cicloColumnas

	xor bl,bl
	sub esi,mask_size
	add esi, line
	inc bh			;parte alta es fila
	cmp bh,byte mask_size
	jne cicloFilas

	mov eax,edi

	convencion_C_fin
