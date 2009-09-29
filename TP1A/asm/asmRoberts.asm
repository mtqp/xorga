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
	ret
%endmacro

%macro getLineSize 2	; getLineSize(width, align)
	mov eax, %1			;eax = width
	mov esi, %2			;esi = 4
	xor edx, edx		;edx = 0		[INSTRUCCION NECESARIA?]
	div esi				;eax = width/align , edx = width%align

	mov eax, %1			;eax = width	[no necesito la división]
	cmp edx, 0			;el ancho era múltiplo de 4?
	je  %%retLine		;si ya era, entonces devuelve el line

	sub eax, edx		;eax = width - width%align
	add eax, esi		;eax = width - witdh%align + align
%%retLine:
	mov line, eax
%endmacro

%macro eax_to_char_sat 0
	cmp eax,255
	jle saturMin
	mov al,255
saturMin:
	cmp eax,0
	jge %%retChar
	mov al, 0
%%retChar:
%endmacro

%macro abs_eax 0
	cmp eax,0
	jg %%retAbs
	neg eax
%%retAbs:
%endmacro

section .data

	RobertsX: db 1,0,0,-1
	RobertsY: db 0,1,-1,0

section .text

	global asmRoberts

asmRoberts:
	convencion_C 8

	%define line		[ebp-4]
	%define yindex		[ebp-8]
	%define align		4
	%define mask_size	2
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]
	%define xorder		[ebp+24]
	%define yorder		[ebp+28]

	getLineSize	width, align	; calcula el ancho de la línea en memoria

; [*] establezco los límites de procesamiento respecto al pixel de src
	sub dword width, mask_size	; width -= mask_size
	sub dword height,mask_size	; height -= mask_size

	mov dword yindex, 0			; yindex = 0 [contador de filas]
	xor edi, edi				; edi = 0 [contador de columnas]
	mov ebx, ptr_src			; puntero al inicio de línea src
	mov esi, ptr_dst			; puntero al inicio de línea dst

;posición de src = yindex*line+edi = ebx+edi
;posición de dst = yindex*(line+1)+edi+1 = esi+edi+1

ciclo:
	lea ecx, [ebx+edi]			; ecx = posición actual de src en memoria
	push dword 0				; guarda una variable temporal para resultado
	push mask_size				; 4to parámetro: tamaño de la máscara
	push RobertsX				; 3er paráemtro: máscara
	push dword line				; 2do parámetro: tamaño de la línea
	push dword ecx				; 1er parámetro: posición en src (top-left)

;;; Esto no lo necesito verificar mas, porq se hace si o si por X y por Y
;	cmp dword xorder, 0			; verifica si tiene que derivar en X
;	je dy						; si no hay que derivar en X salta a dy
	call apply_mask				; aplica la máscara
	abs_eax						; obtiene el valor absoluto del resultado
	mov [ebp-24], eax			; guarda el resultado de derivar en X

;;; Idem arriba
;dy:
;	cmp dword yorder, 0			; verifica si tiene que derivar en Y
;	je continuar				; si no hay que derivar en Y salta a continuar

	mov dword [ebp-32],RobertsY	; coloca el operador de Roberts para Y
	call apply_mask				; aplica la máscara
	abs_eax						; obtiene el valor absoluto del resultado
;continuar:
	add esp, 16					; 'quita' los parámetros del stack
	mov edx, line				; copia temporalmente line a edx
	lea ecx, [esi+edi]			; ecx = posición en dst
	lea ecx, [ecx+edx+1]		; ecx = posición en dst + (1,1)

	pop edx						; obtiene el resultado de derivar en X
	add eax, edx				; suma los resultados
	eax_to_char_sat				; aplica saturación
	mov byte [ecx],	al			; pone el resultado de apply_mask en dst

	inc edi						; aumenta la columna en src
	cmp edi, width				; compara la posición en X con el ancho de la img
	jne ciclo					; repite hasta terminar la columna

	inc dword yindex			; pasa a la siguiente fila
	add ebx, line				; corre el puntero a la fila siguiente
	add esi, line				; corre el puntero a la fila siguiente
	xor edi, edi				; pone la columna en 0
	mov ecx, yindex				; copia yindex a ecx temporalmente para comparar
	cmp ecx, height				; compara la posición en Y con la altura de la img
	jne ciclo					; repite todo el ciclo hasta llegar a la última fila

	convencion_C_fin 8
