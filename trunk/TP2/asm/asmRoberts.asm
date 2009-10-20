
section .text
	global asmRoberts

asmRoberts:
	convencion_C 

	%define line		[ebp-4]
	%define align		4
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]

	xor ecx, ecx			; ecx = 0 [contador de columnas]
	mov esi, ptr_src		; puntero al inicio de línea src
	mov edi, ptr_dst		; puntero al inicio de línea dst

	convencion_C_fin
