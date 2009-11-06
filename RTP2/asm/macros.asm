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

; abs xmm
;	calcula el valor absoluto del registro (packed words)
%macro abs 1
	pxor xmm6,xmm6
	pcmpgtw xmm6,%1
	pxor %1,xmm6
	psubw %1,xmm6
%endmacro

