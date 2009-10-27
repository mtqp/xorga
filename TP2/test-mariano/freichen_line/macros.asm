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

