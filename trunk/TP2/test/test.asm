%include "macros.asm"

extern apply_lineX

section .data
	alguna_cosa: DB 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
section .text
	global _start
_start:
	mov esi, alguna_cosa
	push esi
	push 0
	call apply_lineX
debug:
	add esp, 8
	mov eax, 1
	mov ebx, 0
	int 80h 
