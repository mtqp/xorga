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

section .text
	global  apply_mask

apply_mask:
	convencion_C

	%define imgSrc [ebp+8]
	%define line [ebp+12]
	%define ptr_mask [ebp+16]
	%define mask_size [ebp+20]

	mov esi,imgSrc			;copio la posición del primer pixel
	xor ebx,ebx			;contador que va de 0 a mask_size
	xor edi,edi			;resultado parcial
	mov ecx,ptr_mask		;copio la dirección de la máscara
cicloFilas:

	cicloColumnas:
		movsx dx,[ecx]		;extension de signo de mascara
		xor eax,eax		;limpio eax
		mov al, [esi]		;guardo el primer pixel
		imul dx			;multiplico por el valor de la mascara
		cwde			;extiendo el signo del resultado
		add edi, eax		;sumo el valor al resultado parcial
		inc esi			;me corro de pixel
		inc ecx			;me corro en la máscara

		inc bl			;incremento la columna
		cmp bl,byte mask_size
		jne cicloColumnas

	xor bl,bl			;reseteo el contador de columna
	sub esi,mask_size		;vuelvo al principio de la línea de la máscara
	add esi, line			;me corro una fila en la máscara
	inc bh				;incremento el contador de fila
	cmp bh,byte mask_size
	jne cicloFilas

	mov eax,edi

	convencion_C_fin
